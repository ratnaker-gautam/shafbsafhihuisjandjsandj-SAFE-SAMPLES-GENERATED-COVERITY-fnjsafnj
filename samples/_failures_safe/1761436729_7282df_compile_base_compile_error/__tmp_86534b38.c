//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

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
    char buffer[256];
    size_t buffer_len;
    int processed_count;
    int max_iterations;
};

struct Transition {
    enum State from_state;
    enum State to_state;
    int (*condition)(struct Context*);
};

int condition_idle_to_reading(struct Context* ctx) {
    return ctx->processed_count < ctx->max_iterations;
}

int condition_reading_to_processing(struct Context* ctx) {
    return ctx->buffer_len > 0 && ctx->buffer_len < sizeof(ctx->buffer);
}

int condition_processing_to_writing(struct Context* ctx) {
    return ctx->buffer_len > 0;
}

int condition_writing_to_idle(struct Context* ctx) {
    return ctx->processed_count < ctx->max_iterations;
}

int condition_any_to_error(struct Context* ctx) {
    return ctx->buffer_len >= sizeof(ctx->buffer) || ctx->processed_count >= 1000;
}

int condition_idle_to_done(struct Context* ctx) {
    return ctx->processed_count >= ctx->max_iterations;
}

struct Transition transitions[] = {
    {STATE_IDLE, STATE_READING, condition_idle_to_reading},
    {STATE_READING, STATE_PROCESSING, condition_reading_to_processing},
    {STATE_PROCESSING, STATE_WRITING, condition_processing_to_writing},
    {STATE_WRITING, STATE_IDLE, condition_writing_to_idle},
    {STATE_IDLE, STATE_DONE, condition_idle_to_done},
    {STATE_IDLE, STATE_ERROR, condition_any_to_error},
    {STATE_READING, STATE_ERROR, condition_any_to_error},
    {STATE_PROCESSING, STATE_ERROR, condition_any_to_error},
    {STATE_WRITING, STATE_ERROR, condition_any_to_error}
};

const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

void initialize_context(struct Context* ctx, int max_iter) {
    if (ctx == NULL || max_iter <= 0 || max_iter > 100) {
        return;
    }
    ctx->current_state = STATE_IDLE;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buffer_len = 0;
    ctx->processed_count = 0;
    ctx->max_iterations = max_iter;
}

void execute_idle(struct Context* ctx) {
    printf("State: IDLE (processed: %d/%d)\n", ctx->processed_count, ctx->max_iterations);
}

void execute_reading(struct Context* ctx) {
    printf("Enter text to process: ");
    fflush(stdout);
    
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) != NULL) {
        ctx->buffer_len = strlen(ctx->buffer);
        if (ctx->buffer_len > 0 && ctx->buffer[ctx->buffer_len - 1] == '\n') {
            ctx->buffer[ctx->buffer_len - 1] = '\0';
            ctx->buffer_len--;
        }
        printf("State: READING (read %zu characters)\n", ctx->buffer_len);
    } else {
        ctx->buffer_len = 0;
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
    }
}

void execute_processing(struct Context* ctx) {
    printf("State: PROCESSING\n");
    for (size_t i = 0; i < ctx->buffer_len && i < sizeof(ctx->buffer); i++) {
        if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
            ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
        }
    }
}

void execute_writing(struct Context* ctx) {
    printf("State: WRITING: %s\n", ctx->buffer);
    ctx->processed_count++;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buffer_len = 0;
}

void execute_error(struct Context* ctx) {
    printf("State: ERROR - Invalid state detected\n");
    ctx->current_state = STATE_DONE;
}

void execute_done(struct Context* ctx) {
    printf("State: DONE - Processed %d items\n", ctx->processed_count);
}

enum State get_next_state(struct Context* ctx) {
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].from_state == ctx->current_state) {
            if (transitions[i].condition(ctx)) {
                return transitions[i].to_state;
            }
        }
    }
    return ctx->current_state;
}

void execute_state(struct Context* ctx) {
    switch (ctx->current_state) {
        case STATE_IDLE:
            execute_idle(ctx);
            break;
        case STATE_READING:
            execute_reading(ctx);
            break;
        case STATE_PROCESSING:
            execute_processing(ctx);
            break;
        case STATE_WRITING:
            execute_writing(ctx);
            break;
        case STATE_ERROR:
            execute_error(ctx);
            break;
        case STATE_DONE:
            execute_done(ctx);
            break;
    }
}

int main(void) {
    struct Context ctx;
    int iterations;
    
    printf("Enter number of iterations (1-10): ");
    if (scanf