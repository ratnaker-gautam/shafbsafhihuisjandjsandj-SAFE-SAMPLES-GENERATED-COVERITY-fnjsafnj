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
    int processed_count;
    int max_iterations;
};

struct Transition {
    enum State from_state;
    enum State to_state;
    int (*condition)(struct Context*);
};

void context_init(struct Context* ctx) {
    if (ctx == NULL) return;
    ctx->current_state = STATE_IDLE;
    memset(ctx->input_buffer, 0, sizeof(ctx->input_buffer));
    memset(ctx->output_buffer, 0, sizeof(ctx->output_buffer));
    ctx->input_length = 0;
    ctx->output_length = 0;
    ctx->processed_count = 0;
    ctx->max_iterations = 10;
}

int condition_idle_to_reading(struct Context* ctx) {
    if (ctx == NULL) return 0;
    return ctx->current_state == STATE_IDLE && ctx->processed_count < ctx->max_iterations;
}

int condition_reading_to_processing(struct Context* ctx) {
    if (ctx == NULL) return 0;
    return ctx->current_state == STATE_READING && ctx->input_length > 0 && ctx->input_length < sizeof(ctx->input_buffer);
}

int condition_processing_to_writing(struct Context* ctx) {
    if (ctx == NULL) return 0;
    return ctx->current_state == STATE_PROCESSING && ctx->output_length < sizeof(ctx->output_buffer);
}

int condition_writing_to_idle(struct Context* ctx) {
    if (ctx == NULL) return 0;
    return ctx->current_state == STATE_WRITING;
}

int condition_any_to_error(struct Context* ctx) {
    if (ctx == NULL) return 0;
    return ctx->input_length >= sizeof(ctx->input_buffer) || ctx->output_length >= sizeof(ctx->output_buffer) || ctx->processed_count >= ctx->max_iterations;
}

int condition_error_to_done(struct Context* ctx) {
    if (ctx == NULL) return 0;
    return ctx->current_state == STATE_ERROR;
}

int condition_idle_to_done(struct Context* ctx) {
    if (ctx == NULL) return 0;
    return ctx->current_state == STATE_IDLE && ctx->processed_count >= ctx->max_iterations;
}

struct Transition transitions[] = {
    {STATE_IDLE, STATE_READING, condition_idle_to_reading},
    {STATE_READING, STATE_PROCESSING, condition_reading_to_processing},
    {STATE_PROCESSING, STATE_WRITING, condition_processing_to_writing},
    {STATE_WRITING, STATE_IDLE, condition_writing_to_idle},
    {STATE_IDLE, STATE_DONE, condition_idle_to_done},
    {STATE_READING, STATE_ERROR, condition_any_to_error},
    {STATE_PROCESSING, STATE_ERROR, condition_any_to_error},
    {STATE_WRITING, STATE_ERROR, condition_any_to_error},
    {STATE_ERROR, STATE_DONE, condition_error_to_done}
};

const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

enum State state_machine_step(struct Context* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].from_state == ctx->current_state) {
            if (transitions[i].condition(ctx)) {
                ctx->current_state = transitions[i].to_state;
                break;
            }
        }
    }
    
    return ctx->current_state;
}

void state_idle_action(struct Context* ctx) {
    if (ctx == NULL) return;
    ctx->processed_count++;
}

void state_reading_action(struct Context* ctx) {
    if (ctx == NULL) return;
    printf("Enter text (max %zu chars): ", sizeof(ctx->input_buffer) - 1);
    
    if (fgets(ctx->input_buffer, sizeof(ctx->input_buffer), stdin) != NULL) {
        ctx->input_length = strlen(ctx->input_buffer);
        if (ctx->input_length > 0 && ctx->input_buffer[ctx->input_length - 1] == '\n') {
            ctx->input_buffer[ctx->input_length - 1] = '\0';
            ctx->input_length--;
        }
    } else {
        ctx->input_length = 0;
    }
}

void state_processing_action(struct Context* ctx) {
    if (ctx == NULL) return;
    ctx->output_length = 0;
    
    for (size_t i = 0; i < ctx->input_length && ctx->output_length < sizeof(ctx->output_buffer) - 1; i++) {
        char c = ctx->input_buffer[i];
        if (isalpha((unsigned char)c)) {
            ctx->output_buffer[ctx->output_length++] = toupper((unsigned char)c);
        }
    }
    ctx->output_buffer[ctx->output_length] = '\0';
}

void state_writing_action(struct Context* ctx) {
    if (ctx == NULL) return;
    printf("Processed: %s\n", ctx->output_buffer);
    memset(ctx->input_buffer, 0, sizeof(ctx->input_buffer));