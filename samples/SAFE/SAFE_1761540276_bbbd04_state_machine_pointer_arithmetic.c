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
    if (input == 'B') {
        ctx->pos = 0;
        *(ctx->buffer + ctx->pos) = '\0';
        return STATE_READING;
    }
    return STATE_ERROR;
}

state_t handle_reading(context_t *ctx, char input) {
    if (ctx->pos >= MAX_INPUT_LEN - 1) {
        return STATE_ERROR;
    }
    if (input == 'E') {
        if (ctx->pos > 0) {
            *(ctx->buffer + ctx->pos) = '\0';
            return STATE_PROCESSING;
        }
        return STATE_ERROR;
    }
    if (input >= '0' && input <= '9') {
        *(ctx->buffer + ctx->pos) = input;
        ctx->pos++;
        return STATE_READING;
    }
    return STATE_ERROR;
}

state_t handle_processing(context_t *ctx, char input) {
    if (input != '\n' && input != '\0') {
        return STATE_ERROR;
    }
    int sum = 0;
    char *ptr = ctx->buffer;
    while (*ptr != '\0') {
        if (*ptr >= '0' && *ptr <= '9') {
            sum += (*ptr - '0');
        }
        ptr++;
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

int main(void) {
    context_t ctx;
    ctx.current = STATE_START;
    ctx.pos = 0;
    ctx.result = 0;
    
    printf("Enter sequence (B<digits>E): ");
    
    char input[MAX_INPUT_LEN * 2];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    
    char *ptr = input;
    while (*ptr != '\0' && *ptr != '\n') {
        if (ctx.current >= STATE_COUNT || ctx.current < 0) {
            ctx.current = STATE_ERROR;
            break;
        }
        state_t next = handlers[ctx.current](&ctx, *ptr);
        ctx.current = next;
        ptr++;
    }
    
    if (ctx.current == STATE_FINISHED) {
        printf("Sum of digits: %d\n", ctx.result);
        return EXIT_SUCCESS;
    } else {
        printf("Error: Invalid input sequence\n");
        return EXIT_FAILURE;
    }
}