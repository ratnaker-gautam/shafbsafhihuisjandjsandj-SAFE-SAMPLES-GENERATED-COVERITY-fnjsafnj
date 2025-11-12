//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_LEN 100
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
    char *input;
    size_t position;
    size_t length;
    int result;
} context_t;

static state_t handle_start(context_t *ctx);
static state_t handle_reading(context_t *ctx);
static state_t handle_processing(context_t *ctx);
static state_t handle_final(context_t *ctx);
static state_t handle_error(context_t *ctx);

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 1] = {0};
    context_t ctx = {STATE_START, input_buffer, 0, 0, 0};
    
    printf("Enter a string (max %d chars): ", MAX_INPUT_LEN);
    if (fgets(input_buffer, MAX_INPUT_LEN + 1, stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && *(input_buffer + len - 1) == '\n') {
        *(input_buffer + len - 1) = '\0';
        len--;
    }
    ctx.length = len;
    
    if (ctx.length == 0) {
        fprintf(stderr, "Empty input\n");
        return EXIT_FAILURE;
    }
    
    state_t (*handlers[STATE_COUNT])(context_t *) = {
        handle_start,
        handle_reading,
        handle_processing,
        handle_final,
        handle_error
    };
    
    while (ctx.current != STATE_FINAL && ctx.current != STATE_ERROR) {
        if (ctx.current >= STATE_COUNT) {
            ctx.current = STATE_ERROR;
            break;
        }
        ctx.current = handlers[ctx.current](&ctx);
    }
    
    if (ctx.current == STATE_ERROR) {
        fprintf(stderr, "State machine error\n");
        return EXIT_FAILURE;
    }
    
    printf("Processed result: %d\n", ctx.result);
    return EXIT_SUCCESS;
}

static state_t handle_start(context_t *ctx) {
    if (ctx == NULL || ctx->input == NULL) {
        return STATE_ERROR;
    }
    ctx->position = 0;
    ctx->result = 0;
    return STATE_READING;
}

static state_t handle_reading(context_t *ctx) {
    if (ctx == NULL || ctx->input == NULL) {
        return STATE_ERROR;
    }
    
    if (ctx->position >= ctx->length) {
        return STATE_PROCESSING;
    }
    
    char current_char = *(ctx->input + ctx->position);
    if (current_char == '\0') {
        return STATE_PROCESSING;
    }
    
    ctx->position++;
    return STATE_READING;
}

static state_t handle_processing(context_t *ctx) {
    if (ctx == NULL || ctx->input == NULL) {
        return STATE_ERROR;
    }
    
    if (ctx->position != ctx->length) {
        return STATE_ERROR;
    }
    
    ctx->position = 0;
    while (ctx->position < ctx->length) {
        char current_char = *(ctx->input + ctx->position);
        if (current_char >= '0' && current_char <= '9') {
            int digit = current_char - '0';
            if (ctx->result > (INT_MAX - digit) / 10) {
                return STATE_ERROR;
            }
            ctx->result = ctx->result * 10 + digit;
        }
        ctx->position++;
    }
    
    return STATE_FINAL;
}

static state_t handle_final(context_t *ctx) {
    return STATE_FINAL;
}

static state_t handle_error(context_t *ctx) {
    return STATE_ERROR;
}