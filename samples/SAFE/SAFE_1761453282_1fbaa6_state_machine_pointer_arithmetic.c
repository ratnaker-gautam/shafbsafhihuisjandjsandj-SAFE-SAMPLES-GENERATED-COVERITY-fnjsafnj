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
    state_t current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_pos;
    int processed_count;
} context_t;

state_t handle_start(context_t *ctx, char input);
state_t handle_reading(context_t *ctx, char input);
state_t handle_processing(context_t *ctx, char input);
state_t handle_finished(context_t *ctx, char input);
state_t handle_error(context_t *ctx, char input);

state_t (*state_handlers[STATE_COUNT])(context_t*, char) = {
    handle_start,
    handle_reading,
    handle_processing,
    handle_finished,
    handle_error
};

state_t handle_start(context_t *ctx, char input) {
    if (input == 'B') {
        ctx->buffer_pos = 0;
        *(ctx->buffer + ctx->buffer_pos) = '\0';
        return STATE_READING;
    }
    return STATE_ERROR;
}

state_t handle_reading(context_t *ctx, char input) {
    if (input == 'E') {
        if (ctx->buffer_pos > 0) {
            return STATE_PROCESSING;
        }
        return STATE_ERROR;
    }
    if (ctx->buffer_pos < MAX_INPUT_LEN - 1) {
        if (input >= 'A' && input <= 'Z') {
            *(ctx->buffer + ctx->buffer_pos) = input;
            ctx->buffer_pos++;
            *(ctx->buffer + ctx->buffer_pos) = '\0';
            return STATE_READING;
        }
    }
    return STATE_ERROR;
}

state_t handle_processing(context_t *ctx, char input) {
    if (input == 'P') {
        char *ptr = ctx->buffer;
        while (*ptr != '\0') {
            if (*ptr >= 'A' && *ptr <= 'Z') {
                *ptr = *ptr + ('a' - 'A');
            }
            ptr++;
        }
        ctx->processed_count++;
        return STATE_FINISHED;
    }
    return STATE_ERROR;
}

state_t handle_finished(context_t *ctx, char input) {
    if (input == 'R') {
        return STATE_START;
    }
    if (input == 'Q') {
        return STATE_ERROR;
    }
    return STATE_FINISHED;
}

state_t handle_error(context_t *ctx, char input) {
    if (input == 'R') {
        ctx->buffer_pos = 0;
        ctx->processed_count = 0;
        *(ctx->buffer) = '\0';
        return STATE_START;
    }
    return STATE_ERROR;
}

int main(void) {
    context_t ctx;
    ctx.current_state = STATE_START;
    ctx.buffer_pos = 0;
    ctx.processed_count = 0;
    *(ctx.buffer) = '\0';

    printf("State Machine Started\n");
    printf("Commands: B=Begin, A-Z=Add char, E=End input, P=Process, R=Reset, Q=Quit\n");

    while (1) {
        printf("State: %d, Processed: %d, Buffer: %s\n", 
               ctx.current_state, ctx.processed_count, ctx.buffer);
        printf("Enter command: ");

        char input[4];
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }

        if (strlen(input) == 0 || input[0] == '\n') {
            continue;
        }

        char cmd = input[0];
        if (ctx.current_state < STATE_COUNT && ctx.current_state >= 0) {
            state_t new_state = state_handlers[ctx.current_state](&ctx, cmd);
            ctx.current_state = new_state;
        }

        if (ctx.current_state == STATE_ERROR && cmd != 'R') {
            printf("Invalid operation in current state\n");
        }

        if (cmd == 'Q' && ctx.current_state == STATE_ERROR) {
            break;
        }
    }

    printf("Final count: %d\n", ctx.processed_count);
    return 0;
}