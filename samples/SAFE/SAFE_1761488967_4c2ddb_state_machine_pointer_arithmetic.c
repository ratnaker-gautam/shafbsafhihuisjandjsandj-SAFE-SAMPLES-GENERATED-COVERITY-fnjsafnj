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
        *(ctx->buffer + ctx->buffer_pos) = '\0';
        ctx->buffer_pos = 0;
        return STATE_READING;
    }
    return STATE_ERROR;
}

state_t handle_reading(context_t *ctx, char input) {
    if (input == 'E') {
        if (ctx->buffer_pos > 0) {
            *(ctx->buffer + ctx->buffer_pos) = '\0';
            return STATE_PROCESSING;
        }
        return STATE_ERROR;
    }
    if (ctx->buffer_pos < MAX_INPUT_LEN - 1) {
        *(ctx->buffer + ctx->buffer_pos) = input;
        ctx->buffer_pos++;
        return STATE_READING;
    }
    return STATE_ERROR;
}

state_t handle_processing(context_t *ctx, char input) {
    if (input == 'N') {
        char *ptr = ctx->buffer;
        while (*ptr != '\0') {
            if (*ptr >= 'a' && *ptr <= 'z') {
                *ptr = *ptr - 'a' + 'A';
            }
            ptr++;
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
    return STATE_ERROR;
}

state_t handle_error(context_t *ctx, char input) {
    return STATE_ERROR;
}

int main(void) {
    context_t ctx;
    ctx.current_state = STATE_START;
    ctx.buffer_pos = 0;
    ctx.processed_count = 0;
    
    printf("Enter sequence (B to begin, E to end processing, N to process, R to reset):\n");
    
    int input_char;
    while ((input_char = getchar()) != EOF) {
        if (input_char == '\n') {
            continue;
        }
        
        if (ctx.current_state >= STATE_COUNT) {
            break;
        }
        
        state_t new_state = state_handlers[ctx.current_state](&ctx, (char)input_char);
        ctx.current_state = new_state;
        
        if (ctx.current_state == STATE_FINISHED) {
            printf("Processed: %s (total: %d)\n", ctx.buffer, ctx.processed_count);
        } else if (ctx.current_state == STATE_ERROR) {
            printf("Error in state machine\n");
            break;
        }
    }
    
    printf("Final processed count: %d\n", ctx.processed_count);
    return 0;
}