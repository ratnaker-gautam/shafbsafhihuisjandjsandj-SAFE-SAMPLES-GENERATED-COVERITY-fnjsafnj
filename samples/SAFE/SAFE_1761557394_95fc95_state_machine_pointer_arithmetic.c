//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>

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
    char *input;
    size_t pos;
    size_t len;
    int result;
} context_t;

state_t handle_start(context_t *ctx);
state_t handle_reading(context_t *ctx);
state_t handle_processing(context_t *ctx);
state_t handle_finished(context_t *ctx);
state_t handle_error(context_t *ctx);

state_t (*handlers[STATE_COUNT])(context_t *) = {
    handle_start,
    handle_reading,
    handle_processing,
    handle_finished,
    handle_error
};

state_t handle_start(context_t *ctx) {
    ctx->result = 0;
    ctx->pos = 0;
    return STATE_READING;
}

state_t handle_reading(context_t *ctx) {
    if (ctx->input == NULL) {
        return STATE_ERROR;
    }
    
    if (ctx->pos >= ctx->len) {
        return STATE_ERROR;
    }
    
    if (*(ctx->input + ctx->pos) == '\0') {
        if (ctx->pos == 0) {
            return STATE_ERROR;
        }
        return STATE_PROCESSING;
    }
    
    ctx->pos++;
    return STATE_READING;
}

state_t handle_processing(context_t *ctx) {
    char *ptr = ctx->input;
    int sum = 0;
    
    while (*ptr != '\0' && (size_t)(ptr - ctx->input) < ctx->len) {
        if (*ptr >= '0' && *ptr <= '9') {
            sum += *ptr - '0';
        }
        ptr++;
    }
    
    ctx->result = sum;
    return STATE_FINISHED;
}

state_t handle_finished(context_t *ctx) {
    printf("Processing complete. Sum of digits: %d\n", ctx->result);
    return STATE_FINISHED;
}

state_t handle_error(context_t *ctx) {
    printf("Error occurred during processing.\n");
    return STATE_ERROR;
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 1];
    context_t ctx;
    
    printf("Enter a string (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input.\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("Empty input.\n");
        return EXIT_FAILURE;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        printf("Input too long.\n");
        return EXIT_FAILURE;
    }
    
    ctx.input = input_buffer;
    ctx.len = input_len;
    ctx.current = STATE_START;
    
    while (ctx.current != STATE_FINISHED && ctx.current != STATE_ERROR) {
        if (ctx.current >= STATE_COUNT) {
            printf("Invalid state.\n");
            return EXIT_FAILURE;
        }
        ctx.current = handlers[ctx.current](&ctx);
    }
    
    return EXIT_SUCCESS;
}