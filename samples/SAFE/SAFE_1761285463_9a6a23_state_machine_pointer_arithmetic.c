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
        *(ctx->buffer + ctx->pos) = '\0';
        return STATE_PROCESSING;
    }
    if (input >= '0' && input <= '9') {
        *(ctx->buffer + ctx->pos) = input;
        ctx->pos++;
        return STATE_READING;
    }
    return STATE_ERROR;
}

state_t handle_processing(context_t *ctx, char input) {
    if (input != 'P') {
        return STATE_ERROR;
    }
    int sum = 0;
    char *ptr = ctx->buffer;
    while (*ptr != '\0') {
        if (*ptr < '0' || *ptr > '9') {
            return STATE_ERROR;
        }
        sum += (*ptr - '0');
        if (sum < 0) {
            return STATE_ERROR;
        }
        ptr++;
    }
    ctx->result = sum;
    return STATE_FINISHED;
}

state_t handle_finished(context_t *ctx, char input) {
    if (input == 'X') {
        return STATE_START;
    }
    return STATE_ERROR;
}

state_t handle_error(context_t *ctx, char input) {
    return STATE_ERROR;
}

int main(void) {
    context_t ctx;
    ctx.current = STATE_START;
    ctx.pos = 0;
    ctx.result = 0;
    
    printf("Enter sequence (B<digits>EPX to process): ");
    
    int c;
    while ((c = getchar()) != EOF && c != '\n') {
        if (c < 0 || c > 127) {
            ctx.current = STATE_ERROR;
            break;
        }
        
        state_t next = handlers[ctx.current](&ctx, (char)c);
        ctx.current = next;
        
        if (ctx.current == STATE_ERROR) {
            break;
        }
    }
    
    if (ctx.current == STATE_FINISHED) {
        printf("Result: %d\n", ctx.result);
    } else {
        printf("Error: Invalid sequence\n");
    }
    
    return 0;
}