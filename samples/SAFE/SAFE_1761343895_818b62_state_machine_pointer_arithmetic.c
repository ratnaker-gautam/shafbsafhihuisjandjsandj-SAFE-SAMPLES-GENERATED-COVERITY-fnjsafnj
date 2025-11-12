//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LEN 128
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
    size_t length;
    int processed_count;
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
        ctx->length = 0;
        *(ctx->buffer + ctx->length) = '\0';
        return STATE_READING;
    }
    return STATE_ERROR;
}

state_t handle_reading(context_t *ctx, char input) {
    if (input == 'E') {
        if (ctx->length > 0) {
            return STATE_PROCESSING;
        }
        return STATE_ERROR;
    }
    if (ctx->length < MAX_INPUT_LEN - 1) {
        *(ctx->buffer + ctx->length) = input;
        ctx->length++;
        *(ctx->buffer + ctx->length) = '\0';
        return STATE_READING;
    }
    return STATE_ERROR;
}

state_t handle_processing(context_t *ctx, char input) {
    if (input == 'N') {
        char *ptr = ctx->buffer;
        while (*ptr != '\0') {
            if (*ptr >= 'a' && *ptr <= 'z') {
                *ptr = *ptr - 'a' + 'A';
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
        return STATE_FINISHED;
    }
    return STATE_ERROR;
}

state_t handle_error(context_t *ctx, char input) {
    if (input == 'R') {
        return STATE_START;
    }
    return STATE_ERROR;
}

int main(void) {
    context_t ctx;
    ctx.current = STATE_START;
    ctx.length = 0;
    ctx.processed_count = 0;
    
    printf("State Machine Processor\n");
    printf("Commands: B=Begin, E=End, N=Process, R=Reset, Q=Quit\n");
    printf("Input format: B<text>ENR (repeat) Q to quit\n");
    
    int running = 1;
    while (running) {
        int c = getchar();
        if (c == EOF || c == '\n') {
            continue;
        }
        
        if (c < 0 || c > 127) {
            continue;
        }
        
        state_t next_state = handlers[ctx.current](&ctx, (char)c);
        
        if (next_state == STATE_FINISHED && c == 'Q') {
            running = 0;
            printf("Processed %d strings. Goodbye.\n", ctx.processed_count);
        } else if (next_state == STATE_FINISHED) {
            printf("Processed: %s\n", ctx.buffer);
            printf("Enter R to reset or Q to quit: ");
        } else if (next_state == STATE_ERROR) {
            printf("Error in state machine. Enter R to reset: ");
            ctx.current = STATE_ERROR;
        } else {
            ctx.current = next_state;
        }
        
        fflush(stdout);
    }
    
    return 0;
}