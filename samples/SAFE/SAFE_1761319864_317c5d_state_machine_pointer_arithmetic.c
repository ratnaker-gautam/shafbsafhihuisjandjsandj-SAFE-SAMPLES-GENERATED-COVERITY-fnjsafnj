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
    if (ctx == NULL) return STATE_ERROR;
    
    if (input == 'B') {
        ctx->buffer_pos = 0;
        *(ctx->buffer + ctx->buffer_pos) = '\0';
        return STATE_READING;
    }
    return STATE_START;
}

state_t handle_reading(context_t *ctx, char input) {
    if (ctx == NULL) return STATE_ERROR;
    
    if (input == 'E') {
        if (ctx->buffer_pos > 0) {
            return STATE_PROCESSING;
        }
        return STATE_ERROR;
    }
    
    if (ctx->buffer_pos >= MAX_INPUT_LEN - 1) {
        return STATE_ERROR;
    }
    
    if (input >= 'A' && input <= 'Z') {
        *(ctx->buffer + ctx->buffer_pos) = input;
        ctx->buffer_pos++;
        *(ctx->buffer + ctx->buffer_pos) = '\0';
        return STATE_READING;
    }
    
    return STATE_ERROR;
}

state_t handle_processing(context_t *ctx, char input) {
    if (ctx == NULL) return STATE_ERROR;
    
    if (input == 'C') {
        if (ctx->buffer_pos > 0) {
            char *ptr = ctx->buffer;
            while (*ptr != '\0') {
                if (*ptr >= 'A' && *ptr <= 'Z') {
                    *ptr = (*ptr - 'A' + 1) % 26 + 'A';
                }
                ptr++;
            }
            ctx->processed_count++;
            return STATE_FINISHED;
        }
        return STATE_ERROR;
    }
    return STATE_PROCESSING;
}

state_t handle_finished(context_t *ctx, char input) {
    if (ctx == NULL) return STATE_ERROR;
    
    if (input == 'R') {
        return STATE_START;
    }
    return STATE_FINISHED;
}

state_t handle_error(context_t *ctx, char input) {
    if (ctx == NULL) return STATE_ERROR;
    
    if (input == 'R') {
        return STATE_START;
    }
    return STATE_ERROR;
}

int main(void) {
    context_t ctx;
    ctx.current_state = STATE_START;
    ctx.buffer_pos = 0;
    ctx.processed_count = 0;
    *(ctx.buffer) = '\0';
    
    printf("State Machine Controller\n");
    printf("Commands: B=Begin, A-Z=Data, E=End, C=Process, R=Reset\n");
    printf("Enter commands: ");
    
    int input_char;
    while ((input_char = getchar()) != EOF && input_char != 'Q') {
        if (input_char == '\n') continue;
        
        if (ctx.current_state < STATE_COUNT && ctx.current_state >= 0) {
            state_t new_state = state_handlers[ctx.current_state](&ctx, (char)input_char);
            ctx.current_state = new_state;
        } else {
            ctx.current_state = STATE_ERROR;
        }
        
        switch (ctx.current_state) {
            case STATE_START:
                printf("State: START | Processed: %d\n", ctx.processed_count);
                break;
            case STATE_READING:
                printf("State: READING | Buffer: %s\n", ctx.buffer);
                break;
            case STATE_PROCESSING:
                printf("State: PROCESSING | Buffer: %s\n", ctx.buffer);
                break;
            case STATE_FINISHED:
                printf("State: FINISHED | Result: %s\n", ctx.buffer);
                break;
            case STATE_ERROR:
                printf("State: ERROR | Use R to reset\n");
                break;
        }
        
        printf("Next command: ");
    }
    
    printf("Final processed count: %d\n", ctx.processed_count);
    return 0;
}