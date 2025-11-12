//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
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
    STATE_FINAL,
    STATE_ERROR
} state_t;

typedef struct {
    state_t current;
    char *buffer;
    size_t pos;
    size_t len;
} context_t;

state_t handle_start(context_t *ctx);
state_t handle_reading(context_t *ctx);
state_t handle_processing(context_t *ctx);
state_t handle_final(context_t *ctx);
state_t handle_error(context_t *ctx);

int main(void) {
    char input_buffer[MAX_INPUT_LEN];
    context_t ctx;
    state_t (*handlers[STATE_COUNT])(context_t *) = {
        handle_start,
        handle_reading,
        handle_processing,
        handle_final,
        handle_error
    };
    
    ctx.buffer = input_buffer;
    ctx.pos = 0;
    ctx.len = MAX_INPUT_LEN;
    ctx.current = STATE_START;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN - 1);
    
    while (ctx.current != STATE_FINAL && ctx.current != STATE_ERROR) {
        if (ctx.current >= STATE_START && ctx.current < STATE_COUNT) {
            ctx.current = handlers[ctx.current](&ctx);
        } else {
            ctx.current = STATE_ERROR;
        }
    }
    
    if (ctx.current == STATE_FINAL) {
        printf("Processed result: ");
        char *ptr = ctx.buffer;
        while (ptr < ctx.buffer + ctx.pos) {
            putchar(*ptr);
            ptr++;
        }
        putchar('\n');
    } else {
        printf("Error occurred during processing\n");
    }
    
    return 0;
}

state_t handle_start(context_t *ctx) {
    if (ctx == NULL || ctx->buffer == NULL) {
        return STATE_ERROR;
    }
    
    char *input_ptr = ctx->buffer;
    size_t remaining = ctx->len - 1;
    
    if (fgets(input_ptr, (int)remaining, stdin) == NULL) {
        return STATE_ERROR;
    }
    
    size_t input_len = strlen(input_ptr);
    if (input_len > 0 && *(input_ptr + input_len - 1) == '\n') {
        *(input_ptr + input_len - 1) = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        return STATE_ERROR;
    }
    
    ctx->pos = input_len;
    return STATE_READING;
}

state_t handle_reading(context_t *ctx) {
    if (ctx == NULL || ctx->buffer == NULL || ctx->pos == 0) {
        return STATE_ERROR;
    }
    
    char *start = ctx->buffer;
    char *end = ctx->buffer + ctx->pos;
    
    while (start < end) {
        if (*start < 32 || *start > 126) {
            return STATE_ERROR;
        }
        start++;
    }
    
    return STATE_PROCESSING;
}

state_t handle_processing(context_t *ctx) {
    if (ctx == NULL || ctx->buffer == NULL || ctx->pos == 0) {
        return STATE_ERROR;
    }
    
    char *start = ctx->buffer;
    char *end = ctx->buffer + ctx->pos;
    
    while (start < end) {
        if (*start >= 'a' && *start <= 'z') {
            *start = *start - 'a' + 'A';
        }
        start++;
    }
    
    return STATE_FINAL;
}

state_t handle_final(context_t *ctx) {
    return STATE_FINAL;
}

state_t handle_error(context_t *ctx) {
    return STATE_ERROR;
}