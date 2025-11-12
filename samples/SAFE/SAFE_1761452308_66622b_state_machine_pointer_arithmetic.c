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
    return STATE_ERROR;
}

state_t handle_finished(context_t *ctx, char input) {
    if (input == 'R') {
        ctx->buffer_pos = 0;
        *(ctx->buffer + ctx->buffer_pos) = '\0';
        return STATE_START;
    }
    return STATE_ERROR;
}

state_t handle_error(context_t *ctx, char input) {
    if (input == 'R') {
        ctx->buffer_pos = 0;
        *(ctx->buffer + ctx->buffer_pos) = '\0';
        ctx->processed_count = 0;
        return STATE_START;
    }
    return STATE_ERROR;
}

int main(void) {
    context_t ctx;
    ctx.current_state = STATE_START;
    ctx.buffer_pos = 0;
    ctx.processed_count = 0;
    *(ctx.buffer + ctx.buffer_pos) = '\0';
    
    printf("State Machine Controller\n");
    printf("Commands: B=Begin, E=End, C=Process, R=Reset\n");
    printf("Input uppercase letters between B and E\n");
    printf("Enter 'Q' to quit\n\n");
    
    char input;
    while (1) {
        printf("State: ");
        switch (ctx.current_state) {
            case STATE_START: printf("START"); break;
            case STATE_READING: printf("READING"); break;
            case STATE_PROCESSING: printf("PROCESSING"); break;
            case STATE_FINISHED: printf("FINISHED"); break;
            case STATE_ERROR: printf("ERROR"); break;
        }
        printf(" | Buffer: %s | Processed: %d\n", ctx.buffer, ctx.processed_count);
        printf("Enter command: ");
        
        if (scanf(" %c", &input) != 1) {
            break;
        }
        
        if (input == 'Q') {
            break;
        }
        
        if (ctx.current_state < STATE_COUNT) {
            state_t new_state = state_handlers[ctx.current_state](&ctx, input);
            ctx.current_state = new_state;
        }
        
        if (ctx.current_state == STATE_FINISHED) {
            printf("Processed result: %s\n", ctx.buffer);
        } else if (ctx.current_state == STATE_ERROR) {
            printf("Error occurred. Use 'R' to reset.\n");
        }
    }
    
    printf("Final count: %d strings processed\n", ctx.processed_count);
    return 0;
}