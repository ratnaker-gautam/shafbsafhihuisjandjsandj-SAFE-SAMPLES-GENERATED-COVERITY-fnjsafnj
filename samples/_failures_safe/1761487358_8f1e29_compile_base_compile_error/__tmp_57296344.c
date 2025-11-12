//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

enum State {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_ERROR,
    STATE_DONE
};

struct Context {
    enum State current_state;
    char input_buffer[256];
    char output_buffer[256];
    size_t input_length;
    size_t output_length;
    int error_code;
    int processed_count;
};

struct Transition {
    enum State from_state;
    enum State to_state;
    int (*condition)(struct Context*);
    int (*action)(struct Context*);
};

int condition_has_input(struct Context* ctx) {
    return ctx->input_length > 0;
}

int condition_no_input(struct Context* ctx) {
    return ctx->input_length == 0;
}

int condition_processing_complete(struct Context* ctx) {
    return ctx->processed_count >= ctx->input_length;
}

int condition_error_detected(struct Context* ctx) {
    return ctx->error_code != 0;
}

int action_read_input(struct Context* ctx) {
    printf("Enter text (max 255 chars): ");
    if (fgets(ctx->input_buffer, sizeof(ctx->input_buffer), stdin) == NULL) {
        ctx->error_code = 1;
        return 0;
    }
    ctx->input_length = strlen(ctx->input_buffer);
    if (ctx->input_length > 0 && ctx->input_buffer[ctx->input_length - 1] == '\n') {
        ctx->input_buffer[ctx->input_length - 1] = '\0';
        ctx->input_length--;
    }
    return 1;
}

int action_process_data(struct Context* ctx) {
    if (ctx->input_length >= sizeof(ctx->output_buffer)) {
        ctx->error_code = 2;
        return 0;
    }
    
    ctx->output_length = 0;
    ctx->processed_count = 0;
    
    for (size_t i = 0; i < ctx->input_length && ctx->output_length < sizeof(ctx->output_buffer) - 1; i++) {
        char c = ctx->input_buffer[i];
        if (isalnum((unsigned char)c) || isspace((unsigned char)c)) {
            ctx->output_buffer[ctx->output_length++] = toupper((unsigned char)c);
        } else {
            ctx->output_buffer[ctx->output_length++] = '_';
        }
        ctx->processed_count++;
    }
    ctx->output_buffer[ctx->output_length] = '\0';
    return 1;
}

int action_write_output(struct Context* ctx) {
    printf("Processed output: %s\n", ctx->output_buffer);
    return 1;
}

int action_handle_error(struct Context* ctx) {
    printf("Error %d occurred\n", ctx->error_code);
    ctx->error_code = 0;
    return 1;
}

int action_reset(struct Context* ctx) {
    memset(ctx->input_buffer, 0, sizeof(ctx->input_buffer));
    memset(ctx->output_buffer, 0, sizeof(ctx->output_buffer));
    ctx->input_length = 0;
    ctx->output_length = 0;
    ctx->processed_count = 0;
    ctx->error_code = 0;
    return 1;
}

struct Transition transitions[] = {
    {STATE_IDLE, STATE_READING, condition_no_input, action_read_input},
    {STATE_IDLE, STATE_DONE, condition_has_input, NULL},
    {STATE_READING, STATE_PROCESSING, condition_has_input, action_process_data},
    {STATE_READING, STATE_ERROR, condition_no_input, NULL},
    {STATE_PROCESSING, STATE_WRITING, condition_processing_complete, action_write_output},
    {STATE_PROCESSING, STATE_ERROR, condition_error_detected, NULL},
    {STATE_WRITING, STATE_IDLE, NULL, action_reset},
    {STATE_ERROR, STATE_IDLE, NULL, action_handle_error},
    {STATE_DONE, STATE_IDLE, NULL, action_reset}
};

const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

void run_state_machine(struct Context* ctx) {
    int transition_found;
    
    do {
        transition_found = 0;
        
        for (size_t i = 0; i < num_transitions; i++) {
            struct Transition* t = &transitions[i];
            
            if (t->from_state == ctx->current_state) {
                int condition_met = (t->condition == NULL) ? 1 : t->condition(ctx);
                
                if (condition_met) {
                    ctx->current_state = t->to_state;
                    
                    if (t->action != NULL) {
                        if (!t->action(ctx)) {
                            ctx->current_state = STATE_ERROR;
                        }
                    }
                    
                    transition_found = 1;
                    break;
                }
            }
        }
        
        if (!transition_found && ctx->current_state != STATE_DONE) {
            ctx->current_state = STATE_ERROR;
            transition_found = 1;
        }
        
    } while (transition_found && ctx->current_state != STATE_DONE);
}

int main(void) {
    struct Context ctx;
    memset(&ctx, 0, sizeof(ctx));
    ctx.current_state = STATE_IDLE;
    
    printf("Text Processing State Machine\n");
    printf("Enter empty line to exit\n");
    
    while (ctx.current_state != STATE_DONE) {
        run_state_m