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

int condition_idle_to_reading(struct Context* ctx) {
    if (ctx == NULL) return 0;
    return ctx->input_length == 0 && ctx->processed_count < ctx->max_iterations;
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
    return ctx->input_length >= sizeof(ctx->input_buffer) || ctx->output_length >= sizeof(ctx->output_buffer);
}

int condition_writing_to_idle(struct Context* ctx) {
    if (ctx == NULL) return 0;
    return ctx->output_length == 0;
}

int condition_any_to_done(struct Context* ctx) {
    if (ctx == NULL) return 0;
    return ctx->processed_count >= ctx->max_iterations;
}

int condition_error_to_idle(struct Context* ctx) {
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
    {STATE_IDLE, STATE_DONE, condition_any_to_done},
    {STATE_READING, STATE_DONE, condition_any_to_done},
    {STATE_PROCESSING, STATE_DONE, condition_any_to_done},
    {STATE_WRITING, STATE_DONE, condition_any_to_done}
};

const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

void initialize_context(struct Context* ctx, int max_iter) {
    if (ctx == NULL || max_iter <= 0) return;
    ctx->current_state = STATE_IDLE;
    memset(ctx->input_buffer, 0, sizeof(ctx->input_buffer));
    memset(ctx->output_buffer, 0, sizeof(ctx->output_buffer));
    ctx->input_length = 0;
    ctx->output_length = 0;
    ctx->processed_count = 0;
    ctx->max_iterations = max_iter > 100 ? 100 : max_iter;
}

void read_input(struct Context* ctx) {
    if (ctx == NULL) return;
    printf("Enter text (max 255 chars): ");
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

void process_data(struct Context* ctx) {
    if (ctx == NULL || ctx->input_length == 0) return;
    
    ctx->output_length = 0;
    for (size_t i = 0; i < ctx->input_length && ctx->output_length < sizeof(ctx->output_buffer) - 1; i++) {
        char c = ctx->input_buffer[i];
        if (isalnum((unsigned char)c) || isspace((unsigned char)c)) {
            ctx->output_buffer[ctx->output_length++] = toupper((unsigned char)c);
        }
    }
    ctx->output_buffer[ctx->output_length] = '\0';
    ctx->processed_count++;
    memset(ctx->input_buffer, 0, sizeof(ctx->input_buffer));
    ctx->input_length = 0;
}

void write_output(struct Context* ctx) {
    if (ctx == NULL || ctx->output_length == 0) return;
    printf("Processed: %s\n", ctx->output_buffer);
    memset(ctx->output_buffer, 0, sizeof(ctx->output_buffer));
    ctx->output_length = 0;
}

void handle_error(struct Context* ctx) {
    if (ctx == NULL) return;
    printf("Error: Buffer overflow detected\n");
    memset(ctx->input_buffer, 0, sizeof(ctx->input_buffer));
    memset(ctx->output_buffer, 0, sizeof(ctx->output_buffer));
    ctx->input_length =