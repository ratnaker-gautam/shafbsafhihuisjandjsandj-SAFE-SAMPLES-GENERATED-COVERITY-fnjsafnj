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
};

struct Transition {
    enum State from_state;
    enum State to_state;
    int (*condition)(struct Context*);
};

int condition_idle_to_reading(struct Context* ctx) {
    if (ctx == NULL) return 0;
    return ctx->input_length == 0;
}

int condition_reading_to_processing(struct Context* ctx) {
    if (ctx == NULL) return 0;
    return ctx->input_length > 0 && ctx->input_length < sizeof(ctx->input_buffer);
}

int condition_processing_to_writing(struct Context* ctx) {
    if (ctx == NULL) return 0;
    return ctx->output_length > 0 && ctx->output_length < sizeof(ctx->output_buffer);
}

int condition_processing_to_error(struct Context* ctx) {
    if (ctx == NULL) return 0;
    return ctx->error_code != 0;
}

int condition_writing_to_idle(struct Context* ctx) {
    if (ctx == NULL) return 0;
    return ctx->output_length == 0;
}

int condition_error_to_idle(struct Context* ctx) {
    if (ctx == NULL) return 0;
    return ctx->error_code == 0;
}

int condition_idle_to_done(struct Context* ctx) {
    if (ctx == NULL) return 0;
    return ctx->input_length == 0 && ctx->output_length == 0;
}

struct Transition transitions[] = {
    {STATE_IDLE, STATE_READING, condition_idle_to_reading},
    {STATE_READING, STATE_PROCESSING, condition_reading_to_processing},
    {STATE_PROCESSING, STATE_WRITING, condition_processing_to_writing},
    {STATE_PROCESSING, STATE_ERROR, condition_processing_to_error},
    {STATE_WRITING, STATE_IDLE, condition_writing_to_idle},
    {STATE_ERROR, STATE_IDLE, condition_error_to_idle},
    {STATE_IDLE, STATE_DONE, condition_idle_to_done}
};

const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

void initialize_context(struct Context* ctx) {
    if (ctx == NULL) return;
    ctx->current_state = STATE_IDLE;
    memset(ctx->input_buffer, 0, sizeof(ctx->input_buffer));
    memset(ctx->output_buffer, 0, sizeof(ctx->output_buffer));
    ctx->input_length = 0;
    ctx->output_length = 0;
    ctx->error_code = 0;
}

int read_input(struct Context* ctx) {
    if (ctx == NULL) return -1;
    printf("Enter text (max 255 chars): ");
    if (fgets(ctx->input_buffer, sizeof(ctx->input_buffer), stdin) == NULL) {
        ctx->error_code = 1;
        return -1;
    }
    ctx->input_length = strlen(ctx->input_buffer);
    if (ctx->input_length > 0 && ctx->input_buffer[ctx->input_length - 1] == '\n') {
        ctx->input_buffer[ctx->input_length - 1] = '\0';
        ctx->input_length--;
    }
    return 0;
}

int process_data(struct Context* ctx) {
    if (ctx == NULL || ctx->input_length >= sizeof(ctx->input_buffer)) return -1;
    ctx->output_length = 0;
    for (size_t i = 0; i < ctx->input_length; i++) {
        if (ctx->output_length >= sizeof(ctx->output_buffer) - 1) {
            ctx->error_code = 2;
            return -1;
        }
        ctx->output_buffer[ctx->output_length++] = toupper((unsigned char)ctx->input_buffer[i]);
    }
    ctx->output_buffer[ctx->output_length] = '\0';
    return 0;
}

int write_output(struct Context* ctx) {
    if (ctx == NULL || ctx->output_length >= sizeof(ctx->output_buffer)) return -1;
    printf("Processed: %s\n", ctx->output_buffer);
    ctx->output_length = 0;
    return 0;
}

void handle_error(struct Context* ctx) {
    if (ctx == NULL) return;
    printf("Error %d occurred\n", ctx->error_code);
    ctx->error_code = 0;
}

enum State get_next_state(struct Context* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].from_state == ctx->current_state) {
            if (transitions[i].condition(ctx)) {
                return transitions[i].to_state;
            }
        }
    }
    return ctx->current_state;
}

void execute_state_action(struct Context* ctx) {
    if (ctx == NULL) return;
    switch (ctx->current_state) {
        case STATE_READING:
            read_input(ctx);
            break;
        case STATE_PROCESSING:
            process_data(ctx);
            break;
        case STATE_WRITING:
            write_output(ctx);
            break;
        case STATE_ERROR: