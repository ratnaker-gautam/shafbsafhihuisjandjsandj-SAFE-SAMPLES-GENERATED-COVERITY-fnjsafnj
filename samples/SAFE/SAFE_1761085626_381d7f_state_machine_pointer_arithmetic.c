//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LEN 256
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

state_t handle_start(context_t *ctx, char input);
state_t handle_reading(context_t *ctx, char input);
state_t handle_processing(context_t *ctx, char input);
state_t handle_finished(context_t *ctx, char input);
state_t handle_error(context_t *ctx, char input);

int main(void) {
    context_t ctx;
    char input[MAX_INPUT_LEN * 2];
    state_t (*handlers[STATE_COUNT])(context_t*, char) = {
        handle_start,
        handle_reading,
        handle_processing,
        handle_finished,
        handle_error
    };
    
    memset(&ctx, 0, sizeof(ctx));
    ctx.current = STATE_START;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN - 1);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    
    char *input_ptr = input;
    while (*input_ptr != '\0' && ctx.current != STATE_FINISHED && ctx.current != STATE_ERROR) {
        if (ctx.current < STATE_COUNT) {
            ctx.current = handlers[ctx.current](&ctx, *input_ptr);
        } else {
            ctx.current = STATE_ERROR;
        }
        input_ptr++;
    }
    
    if (ctx.current == STATE_FINISHED) {
        printf("Processing complete. Result: %d\n", ctx.result);
        return EXIT_SUCCESS;
    } else {
        printf("Processing failed or incomplete.\n");
        return EXIT_FAILURE;
    }
}

state_t handle_start(context_t *ctx, char input) {
    if (ctx == NULL) {
        return STATE_ERROR;
    }
    
    if (input == ' ' || input == '\t' || input == '\n') {
        return STATE_START;
    }
    
    ctx->pos = 0;
    *(ctx->buffer + ctx->pos) = input;
    ctx->pos++;
    return STATE_READING;
}

state_t handle_reading(context_t *ctx, char input) {
    if (ctx == NULL) {
        return STATE_ERROR;
    }
    
    if (input == '\n' || input == '\0') {
        if (ctx->pos > 0) {
            *(ctx->buffer + ctx->pos) = '\0';
            return STATE_PROCESSING;
        }
        return STATE_ERROR;
    }
    
    if (ctx->pos >= MAX_INPUT_LEN - 1) {
        return STATE_ERROR;
    }
    
    *(ctx->buffer + ctx->pos) = input;
    ctx->pos++;
    return STATE_READING;
}

state_t handle_processing(context_t *ctx, char input) {
    if (ctx == NULL) {
        return STATE_ERROR;
    }
    
    int sum = 0;
    char *buf_ptr = ctx->buffer;
    while (*buf_ptr != '\0') {
        if (*buf_ptr >= '0' && *buf_ptr <= '9') {
            sum += *buf_ptr - '0';
        }
        buf_ptr++;
    }
    
    ctx->result = sum;
    return STATE_FINISHED;
}

state_t handle_finished(context_t *ctx, char input) {
    return STATE_FINISHED;
}

state_t handle_error(context_t *ctx, char input) {
    return STATE_ERROR;
}