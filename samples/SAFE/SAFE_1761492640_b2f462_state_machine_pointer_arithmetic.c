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
    if (input == 'B') {
        ctx->pos = 0;
        *(ctx->buffer + ctx->pos) = input;
        ctx->pos++;
        return STATE_READING;
    }
    return STATE_ERROR;
}

state_t handle_reading(context_t *ctx, char input) {
    if (input == 'E' && ctx->pos > 1) {
        *(ctx->buffer + ctx->pos) = '\0';
        return STATE_PROCESSING;
    } else if (ctx->pos < MAX_INPUT_LEN - 1 && input >= 'A' && input <= 'Z') {
        *(ctx->buffer + ctx->pos) = input;
        ctx->pos++;
        return STATE_READING;
    }
    return STATE_ERROR;
}

state_t handle_processing(context_t *ctx) {
    char *ptr = ctx->buffer + 1;
    int count = 0;
    
    while (*ptr != 'E' && ptr < ctx->buffer + ctx->pos) {
        if (*ptr == 'X') {
            count++;
        }
        ptr++;
    }
    
    ctx->result = count;
    return STATE_FINISHED;
}

int main(void) {
    context_t ctx;
    char input[MAX_INPUT_LEN * 2];
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0 || input_len >= MAX_INPUT_LEN) {
        return EXIT_FAILURE;
    }
    
    ctx.current = STATE_START;
    ctx.pos = 0;
    ctx.result = 0;
    
    char *input_ptr = input;
    while (*input_ptr != '\0' && ctx.current != STATE_FINISHED && ctx.current != STATE_ERROR) {
        state_t (*handlers[STATE_COUNT])(context_t*, char) = {
            handle_start,
            handle_reading,
            NULL,
            NULL,
            NULL
        };
        
        if (ctx.current == STATE_PROCESSING) {
            ctx.current = handle_processing(&ctx);
            break;
        }
        
        if (handlers[ctx.current] != NULL) {
            ctx.current = handlers[ctx.current](&ctx, *input_ptr);
        } else {
            ctx.current = STATE_ERROR;
            break;
        }
        
        input_ptr++;
    }
    
    if (ctx.current == STATE_FINISHED) {
        printf("Processed successfully. Count: %d\n", ctx.result);
        return EXIT_SUCCESS;
    } else {
        printf("Error processing input\n");
        return EXIT_FAILURE;
    }
}