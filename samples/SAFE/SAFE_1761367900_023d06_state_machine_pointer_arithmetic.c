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
    if (input == 'E') {
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

state_t handle_processing(context_t *ctx, char input) {
    if (input != '\n') {
        return STATE_ERROR;
    }
    char *ptr = ctx->buffer + 1;
    int count = 0;
    while (*ptr && ptr < ctx->buffer + ctx->pos) {
        if (*ptr >= 'a' && *ptr <= 'z') {
            count++;
        }
        ptr++;
    }
    ctx->result = count;
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
    memset(ctx.buffer, 0, sizeof(ctx.buffer));

    printf("Enter sequence (Begin with 'B', end with 'E'): ");
    fflush(stdout);

    int c;
    while ((c = getchar()) != EOF && ctx.current != STATE_FINISHED && ctx.current != STATE_ERROR) {
        if (c == '\r') {
            continue;
        }
        state_t next = handlers[ctx.current](&ctx, (char)c);
        ctx.current = next;
    }

    if (ctx.current == STATE_FINISHED) {
        printf("Processed successfully. Lowercase count: %d\n", ctx.result);
        return EXIT_SUCCESS;
    } else {
        printf("Error: Invalid input sequence\n");
        return EXIT_FAILURE;
    }
}