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
    char *input;
    size_t position;
    size_t length;
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
    if (ctx == NULL) return STATE_ERROR;
    ctx->position = 0;
    ctx->result = 0;
    printf("State machine started. Enter text: ");
    return STATE_READING;
}

state_t handle_reading(context_t *ctx) {
    if (ctx == NULL || ctx->input == NULL) return STATE_ERROR;
    
    char buffer[MAX_INPUT_LEN + 2];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return STATE_ERROR;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && *(buffer + len - 1) == '\n') {
        *(buffer + len - 1) = '\0';
        len--;
    }
    
    if (len >= MAX_INPUT_LEN) {
        return STATE_ERROR;
    }
    
    if (len == 0) {
        return STATE_ERROR;
    }
    
    char *src = buffer;
    char *dst = ctx->input;
    while (*src != '\0') {
        *dst = *src;
        src++;
        dst++;
    }
    *dst = '\0';
    ctx->length = len;
    
    return STATE_PROCESSING;
}

state_t handle_processing(context_t *ctx) {
    if (ctx == NULL || ctx->input == NULL) return STATE_ERROR;
    
    int sum = 0;
    char *ptr = ctx->input;
    while (*ptr != '\0') {
        if (*ptr >= '0' && *ptr <= '9') {
            sum += (*ptr - '0');
        }
        ptr++;
    }
    ctx->result = sum;
    
    return STATE_FINISHED;
}

state_t handle_finished(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    printf("Processing complete. Sum of digits: %d\n", ctx->result);
    return STATE_FINISHED;
}

state_t handle_error(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    printf("Error occurred during processing.\n");
    return STATE_ERROR;
}

int main(void) {
    context_t ctx;
    char input_buffer[MAX_INPUT_LEN + 1];
    
    ctx.input = input_buffer;
    ctx.current = STATE_START;
    
    while (ctx.current != STATE_FINISHED && ctx.current != STATE_ERROR) {
        if (ctx.current >= STATE_COUNT) {
            ctx.current = STATE_ERROR;
            break;
        }
        
        state_t (*handler)(context_t *) = *(handlers + ctx.current);
        if (handler == NULL) {
            ctx.current = STATE_ERROR;
            break;
        }
        
        state_t next_state = handler(&ctx);
        if (next_state >= STATE_COUNT) {
            ctx.current = STATE_ERROR;
            break;
        }
        ctx.current = next_state;
    }
    
    return ctx.current == STATE_FINISHED ? EXIT_SUCCESS : EXIT_FAILURE;
}