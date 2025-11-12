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
    if (input == 'E') {
        if (ctx->pos > 0) {
            *(ctx->buffer + ctx->pos) = '\0';
            return STATE_PROCESSING;
        }
        return STATE_ERROR;
    }
    if (ctx->pos < MAX_INPUT_LEN - 1) {
        if (input >= '0' && input <= '9') {
            *(ctx->buffer + ctx->pos) = input;
            ctx->pos++;
            return STATE_READING;
        }
    }
    return STATE_ERROR;
}

state_t handle_processing(context_t *ctx, char input) {
    if (input == 'C') {
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
    return STATE_ERROR;
}

state_t handle_finished(context_t *ctx, char input) {
    if (input == 'X') {
        return STATE_START;
    }
    return STATE_FINISHED;
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
    ctx.pos = 0;
    ctx.result = 0;
    
    printf("State Machine Started\n");
    printf("Protocol: B[digits]ECX to process, R to reset from error\n");
    printf("Enter commands: ");
    
    int c;
    while ((c = getchar()) != EOF && c != '\n') {
        if (c < 0 || c > 127) {
            continue;
        }
        
        state_t next_state = handlers[ctx.current](&ctx, (char)c);
        ctx.current = next_state;
        
        if (ctx.current == STATE_FINISHED) {
            printf("Result: %d\n", ctx.result);
            printf("Enter X to restart: ");
        } else if (ctx.current == STATE_ERROR) {
            printf("Error! Enter R to reset: ");
        }
        
        int next_char = getchar();
        if (next_char == EOF || next_char == '\n') {
            break;
        }
        ungetc(next_char, stdin);
    }
    
    if (ctx.current == STATE_FINISHED) {
        printf("Final result: %d\n", ctx.result);
    } else if (ctx.current != STATE_START) {
        printf("Incomplete operation\n");
    }
    
    return 0;
}