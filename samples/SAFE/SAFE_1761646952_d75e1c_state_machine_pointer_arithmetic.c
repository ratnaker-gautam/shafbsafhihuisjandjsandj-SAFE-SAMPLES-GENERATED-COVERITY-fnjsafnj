//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LEN 100
#define STATE_COUNT 5

typedef enum {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_FINISHED,
    STATE_ERROR
} state_t;

typedef struct {
    state_t current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_pos;
    int processed_count;
} context_t;

state_t handle_start(context_t *ctx, char input);
state_t handle_reading(context_t *ctx, char input);
state_t handle_processing(context_t *ctx, char input);
state_t handle_finished(context_t *ctx, char input);
state_t handle_error(context_t *ctx, char input);

state_t (*state_handlers[STATE_COUNT])(context_t*, char) = {
    handle_start,
    handle_reading,
    handle_processing,
    handle_finished,
    handle_error
};

state_t handle_start(context_t *ctx, char input) {
    if (input == 'B') {
        ctx->buffer_pos = 0;
        *(ctx->buffer + ctx->buffer_pos) = '\0';
        return STATE_READING;
    }
    return STATE_ERROR;
}

state_t handle_reading(context_t *ctx, char input) {
    if (input == 'E') {
        if (ctx->buffer_pos > 0) {
            return STATE_PROCESSING;
        }
        return STATE_ERROR;
    }
    if (ctx->buffer_pos < MAX_INPUT_LEN - 1) {
        *(ctx->buffer + ctx->buffer_pos) = input;
        ctx->buffer_pos++;
        *(ctx->buffer + ctx->buffer_pos) = '\0';
        return STATE_READING;
    }
    return STATE_ERROR;
}

state_t handle_processing(context_t *ctx, char input) {
    if (input == 'N') {
        size_t len = ctx->buffer_pos;
        if (len > 0) {
            char *start = ctx->buffer;
            char *end = ctx->buffer + len - 1;
            while (start < end) {
                char temp = *start;
                *start = *end;
                *end = temp;
                start++;
                end--;
            }
        }
        ctx->processed_count++;
        return STATE_FINISHED;
    }
    return STATE_ERROR;
}

state_t handle_finished(context_t *ctx, char input) {
    if (input == 'R') {
        return STATE_START;
    }
    if (input == 'Q') {
        return STATE_ERROR;
    }
    return STATE_FINISHED;
}

state_t handle_error(context_t *ctx, char input) {
    return STATE_ERROR;
}

void process_input(context_t *ctx, const char *input) {
    const char *ptr = input;
    while (*ptr != '\0') {
        if (ctx->current_state >= STATE_COUNT || ctx->current_state < 0) {
            ctx->current_state = STATE_ERROR;
            break;
        }
        state_t next_state = state_handlers[ctx->current_state](ctx, *ptr);
        ctx->current_state = next_state;
        ptr++;
    }
}

void print_state_info(const context_t *ctx) {
    switch (ctx->current_state) {
        case STATE_START:
            printf("State: START\n");
            break;
        case STATE_READING:
            printf("State: READING, Buffer: %s\n", ctx->buffer);
            break;
        case STATE_PROCESSING:
            printf("State: PROCESSING, Buffer: %s\n", ctx->buffer);
            break;
        case STATE_FINISHED:
            printf("State: FINISHED, Processed: %d, Result: %s\n", ctx->processed_count, ctx->buffer);
            break;
        case STATE_ERROR:
            printf("State: ERROR\n");
            break;
    }
}

int main(void) {
    context_t ctx;
    ctx.current_state = STATE_START;
    ctx.buffer_pos = 0;
    ctx.processed_count = 0;
    *(ctx.buffer) = '\0';

    char input[MAX_INPUT_LEN * 2];
    
    printf("Enter commands (B to begin, E to end reading, N to process, R to reset, Q to quit):\n");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    process_input(&ctx, input);
    print_state_info(&ctx);
    
    return 0;
}