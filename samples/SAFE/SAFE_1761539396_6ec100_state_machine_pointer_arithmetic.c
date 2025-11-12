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
    state_t current;
    char buffer[MAX_INPUT_LEN];
    size_t pos;
    int result;
} context_t;

state_t handle_start(context_t *ctx, char input) {
    if (input == 'B' || input == 'b') {
        *(ctx->buffer + ctx->pos) = input;
        ctx->pos++;
        return STATE_READING;
    }
    return STATE_ERROR;
}

state_t handle_reading(context_t *ctx, char input) {
    if (ctx->pos >= MAX_INPUT_LEN - 1) {
        return STATE_ERROR;
    }
    if (input == 'E' || input == 'e') {
        *(ctx->buffer + ctx->pos) = '\0';
        return STATE_PROCESSING;
    }
    if ((input >= 'A' && input <= 'Z') || (input >= 'a' && input <= 'z')) {
        *(ctx->buffer + ctx->pos) = input;
        ctx->pos++;
        return STATE_READING;
    }
    return STATE_ERROR;
}

state_t handle_processing(context_t *ctx) {
    char *ptr = ctx->buffer;
    int count = 0;
    
    while (*ptr != '\0') {
        if (*ptr == 'A' || *ptr == 'a') {
            count++;
        }
        ptr++;
    }
    
    ctx->result = count;
    return STATE_FINISHED;
}

state_t (*state_handlers[STATE_COUNT])(context_t*, char) = {
    handle_start,
    handle_reading,
    NULL,
    NULL,
    NULL
};

int main(void) {
    context_t ctx;
    char input[MAX_INPUT_LEN * 2];
    state_t next_state;
    
    memset(&ctx, 0, sizeof(ctx));
    ctx.current = STATE_START;
    
    printf("Enter sequence (start with B/b, end with E/e, letters only): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    char *input_ptr = input;
    while (*input_ptr != '\0' && *input_ptr != '\n') {
        if (ctx.current >= STATE_COUNT) {
            printf("Invalid state\n");
            return 1;
        }
        
        if (ctx.current == STATE_PROCESSING) {
            next_state = handle_processing(&ctx);
        } else if (ctx.current == STATE_FINISHED || ctx.current == STATE_ERROR) {
            break;
        } else {
            if (state_handlers[ctx.current] == NULL) {
                printf("No handler for state\n");
                return 1;
            }
            next_state = state_handlers[ctx.current](&ctx, *input_ptr);
        }
        
        if (next_state >= STATE_COUNT) {
            printf("Invalid next state\n");
            return 1;
        }
        
        ctx.current = next_state;
        input_ptr++;
        
        if (ctx.current == STATE_FINISHED) {
            break;
        }
    }
    
    if (ctx.current == STATE_FINISHED) {
        printf("Processing complete. Count of 'A's: %d\n", ctx.result);
        return 0;
    } else if (ctx.current == STATE_ERROR) {
        printf("Error processing input\n");
        return 1;
    } else {
        printf("Unexpected termination\n");
        return 1;
    }
}