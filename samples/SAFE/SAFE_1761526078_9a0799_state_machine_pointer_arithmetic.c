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

state_t handle_start(context_t *ctx, char input);
state_t handle_reading(context_t *ctx, char input);
state_t handle_processing(context_t *ctx, char input);
state_t handle_finished(context_t *ctx, char input);
state_t handle_error(context_t *ctx, char input);

state_t (*handlers[STATE_COUNT])(context_t*, char) = {
    handle_start,
    handle_reading,
    handle_processing,
    handle_finished,
    handle_error
};

state_t handle_start(context_t *ctx, char input) {
    if (ctx == NULL) return STATE_ERROR;
    
    ctx->pos = 0;
    ctx->result = 0;
    memset(ctx->buffer, 0, MAX_INPUT_LEN);
    
    if (input == 'B' || input == 'b') {
        return STATE_READING;
    }
    return STATE_ERROR;
}

state_t handle_reading(context_t *ctx, char input) {
    if (ctx == NULL) return STATE_ERROR;
    
    if (input >= '0' && input <= '9') {
        if (ctx->pos < MAX_INPUT_LEN - 1) {
            *(ctx->buffer + ctx->pos) = input;
            ctx->pos++;
            return STATE_READING;
        }
        return STATE_ERROR;
    }
    
    if (input == 'E' || input == 'e') {
        if (ctx->pos > 0) {
            *(ctx->buffer + ctx->pos) = '\0';
            return STATE_PROCESSING;
        }
        return STATE_ERROR;
    }
    
    return STATE_ERROR;
}

state_t handle_processing(context_t *ctx, char input) {
    if (ctx == NULL) return STATE_ERROR;
    
    char *ptr = ctx->buffer;
    int value = 0;
    
    while (*ptr != '\0') {
        if (*ptr < '0' || *ptr > '9') {
            return STATE_ERROR;
        }
        value = value * 10 + (*ptr - '0');
        if (value < 0) return STATE_ERROR;
        ptr++;
    }
    
    ctx->result = value;
    return STATE_FINISHED;
}

state_t handle_finished(context_t *ctx, char input) {
    if (ctx == NULL) return STATE_ERROR;
    return STATE_FINISHED;
}

state_t handle_error(context_t *ctx, char input) {
    if (ctx == NULL) return STATE_ERROR;
    return STATE_ERROR;
}

int main(void) {
    context_t ctx;
    char input[MAX_INPUT_LEN * 2];
    size_t input_len;
    
    printf("Enter sequence (Begin with B/b, digits, End with E/e): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    ctx.current = STATE_START;
    
    char *ptr = input;
    while (*ptr != '\0' && ctx.current != STATE_FINISHED && ctx.current != STATE_ERROR) {
        if (ptr >= input + sizeof(input)) break;
        
        state_t next_state = handlers[ctx.current](&ctx, *ptr);
        ctx.current = next_state;
        ptr++;
    }
    
    if (ctx.current == STATE_FINISHED) {
        printf("Success! Processed value: %d\n", ctx.result);
        return 0;
    } else {
        printf("Error: Invalid sequence\n");
        return 1;
    }
}