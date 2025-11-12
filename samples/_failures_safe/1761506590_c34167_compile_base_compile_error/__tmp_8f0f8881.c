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
    enum State from;
    enum State to;
    int (*condition)(struct Context*);
    int (*action)(struct Context*);
};

void context_init(struct Context* ctx) {
    if (ctx == NULL) return;
    ctx->current_state = STATE_IDLE;
    memset(ctx->input_buffer, 0, sizeof(ctx->input_buffer));
    memset(ctx->output_buffer, 0, sizeof(ctx->output_buffer));
    ctx->input_length = 0;
    ctx->output_length = 0;
    ctx->processed_count = 0;
    ctx->max_iterations = 5;
}

int condition_idle_to_reading(struct Context* ctx) {
    if (ctx == NULL) return 0;
    return ctx->current_state == STATE_IDLE && ctx->processed_count < ctx->max_iterations;
}

int action_idle_to_reading(struct Context* ctx) {
    if (ctx == NULL) return 0;
    printf("Enter text to process: ");
    if (fgets(ctx->input_buffer, sizeof(ctx->input_buffer), stdin) == NULL) {
        return 0;
    }
    ctx->input_length = strlen(ctx->input_buffer);
    if (ctx->input_length > 0 && ctx->input_buffer[ctx->input_length - 1] == '\n') {
        ctx->input_buffer[ctx->input_length - 1] = '\0';
        ctx->input_length--;
    }
    return 1;
}

int condition_reading_to_processing(struct Context* ctx) {
    if (ctx == NULL) return 0;
    return ctx->current_state == STATE_READING && ctx->input_length > 0;
}

int action_reading_to_processing(struct Context* ctx) {
    if (ctx == NULL) return 0;
    ctx->output_length = 0;
    for (size_t i = 0; i < ctx->input_length && ctx->output_length < sizeof(ctx->output_buffer) - 1; i++) {
        if (isalnum((unsigned char)ctx->input_buffer[i])) {
            ctx->output_buffer[ctx->output_length++] = toupper((unsigned char)ctx->input_buffer[i]);
        }
    }
    ctx->output_buffer[ctx->output_length] = '\0';
    return 1;
}

int condition_processing_to_writing(struct Context* ctx) {
    if (ctx == NULL) return 0;
    return ctx->current_state == STATE_PROCESSING && ctx->output_length < sizeof(ctx->output_buffer);
}

int action_processing_to_writing(struct Context* ctx) {
    if (ctx == NULL) return 0;
    printf("Processed output: %s\n", ctx->output_buffer);
    ctx->processed_count++;
    return 1;
}

int condition_writing_to_idle(struct Context* ctx) {
    if (ctx == NULL) return 0;
    return ctx->current_state == STATE_WRITING && ctx->processed_count < ctx->max_iterations;
}

int action_writing_to_idle(struct Context* ctx) {
    if (ctx == NULL) return 0;
    memset(ctx->input_buffer, 0, sizeof(ctx->input_buffer));
    memset(ctx->output_buffer, 0, sizeof(ctx->output_buffer));
    ctx->input_length = 0;
    ctx->output_length = 0;
    return 1;
}

int condition_writing_to_done(struct Context* ctx) {
    if (ctx == NULL) return 0;
    return ctx->current_state == STATE_WRITING && ctx->processed_count >= ctx->max_iterations;
}

int action_writing_to_done(struct Context* ctx) {
    if (ctx == NULL) return 0;
    printf("Processing complete. Total iterations: %d\n", ctx->processed_count);
    return 1;
}

int condition_any_to_error(struct Context* ctx) {
    if (ctx == NULL) return 0;
    return ctx->input_length >= sizeof(ctx->input_buffer) || ctx->output_length >= sizeof(ctx->output_buffer);
}

int action_any_to_error(struct Context* ctx) {
    if (ctx == NULL) return 0;
    printf("Error: Buffer overflow detected\n");
    return 1;
}

struct Transition transitions[] = {
    {STATE_IDLE, STATE_READING, condition_idle_to_reading, action_idle_to_reading},
    {STATE_READING, STATE_PROCESSING, condition_reading_to_processing, action_reading_to_processing},
    {STATE_PROCESSING, STATE_WRITING, condition_processing_to_writing, action_processing_to_writing},
    {STATE_WRITING, STATE_IDLE, condition_writing_to_idle, action_writing_to_idle},
    {STATE_WRITING, STATE_DONE, condition_writing_to_done, action_writing_to_done},
    {STATE_IDLE, STATE_ERROR, condition_any_to_error, action_any_to_error},
    {STATE_READING, STATE_ERROR, condition_any_to_error, action_any_to_error},
    {STATE_PROCESSING,