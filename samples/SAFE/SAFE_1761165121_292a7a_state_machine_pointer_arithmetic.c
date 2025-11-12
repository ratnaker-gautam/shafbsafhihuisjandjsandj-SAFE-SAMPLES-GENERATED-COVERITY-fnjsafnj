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
    STATE_FINALIZING,
    STATE_DONE
} state_t;

typedef struct {
    state_t current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_pos;
    size_t processed_chars;
} context_t;

state_t handle_start(context_t *ctx, char input);
state_t handle_reading(context_t *ctx, char input);
state_t handle_processing(context_t *ctx, char input);
state_t handle_finalizing(context_t *ctx, char input);
state_t handle_done(context_t *ctx, char input);

int main(void) {
    context_t ctx;
    ctx.current_state = STATE_START;
    ctx.buffer_pos = 0;
    ctx.processed_chars = 0;
    memset(ctx.buffer, 0, sizeof(ctx.buffer));

    char input[MAX_INPUT_LEN];
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }

    size_t input_len = strlen(input);
    if (input_len > 0 && *(input + input_len - 1) == '\n') {
        *(input + input_len - 1) = '\0';
        input_len--;
    }

    char *input_ptr = input;
    while (*input_ptr != '\0' && ctx.current_state != STATE_DONE) {
        state_t (*handlers[STATE_COUNT])(context_t*, char) = {
            handle_start,
            handle_reading,
            handle_processing,
            handle_finalizing,
            handle_done
        };

        if (ctx.current_state >= STATE_COUNT) {
            fprintf(stderr, "Invalid state\n");
            return EXIT_FAILURE;
        }

        state_t next_state = handlers[ctx.current_state](&ctx, *input_ptr);
        ctx.current_state = next_state;
        input_ptr++;
    }

    if (ctx.current_state != STATE_DONE) {
        ctx.current_state = handle_done(&ctx, '\0');
    }

    printf("Processed %zu characters\n", ctx.processed_chars);
    printf("Final buffer: %s\n", ctx.buffer);

    return EXIT_SUCCESS;
}

state_t handle_start(context_t *ctx, char input) {
    if (ctx == NULL) {
        return STATE_DONE;
    }

    if (input == ' ' || input == '\t') {
        return STATE_START;
    }

    if (ctx->buffer_pos >= MAX_INPUT_LEN - 1) {
        return STATE_DONE;
    }

    *(ctx->buffer + ctx->buffer_pos) = input;
    ctx->buffer_pos++;
    ctx->processed_chars++;
    return STATE_READING;
}

state_t handle_reading(context_t *ctx, char input) {
    if (ctx == NULL) {
        return STATE_DONE;
    }

    if (input == ' ' || input == '\t') {
        return STATE_PROCESSING;
    }

    if (ctx->buffer_pos >= MAX_INPUT_LEN - 1) {
        return STATE_DONE;
    }

    *(ctx->buffer + ctx->buffer_pos) = input;
    ctx->buffer_pos++;
    ctx->processed_chars++;
    return STATE_READING;
}

state_t handle_processing(context_t *ctx, char input) {
    if (ctx == NULL) {
        return STATE_DONE;
    }

    if (input == ' ' || input == '\t') {
        return STATE_PROCESSING;
    }

    if (ctx->buffer_pos >= MAX_INPUT_LEN - 1) {
        return STATE_DONE;
    }

    *(ctx->buffer + ctx->buffer_pos) = ' ';
    ctx->buffer_pos++;
    *(ctx->buffer + ctx->buffer_pos) = input;
    ctx->buffer_pos++;
    ctx->processed_chars += 2;
    return STATE_READING;
}

state_t handle_finalizing(context_t *ctx, char input) {
    if (ctx == NULL) {
        return STATE_DONE;
    }

    if (ctx->buffer_pos >= MAX_INPUT_LEN - 1) {
        return STATE_DONE;
    }

    *(ctx->buffer + ctx->buffer_pos) = '\0';
    return STATE_DONE;
}

state_t handle_done(context_t *ctx, char input) {
    if (ctx != NULL && ctx->buffer_pos < MAX_INPUT_LEN) {
        *(ctx->buffer + ctx->buffer_pos) = '\0';
    }
    return STATE_DONE;
}