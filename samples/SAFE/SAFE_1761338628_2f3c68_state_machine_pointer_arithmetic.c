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
    STATE_OUTPUT,
    STATE_END
} state_t;

typedef struct {
    state_t current;
    char buffer[MAX_INPUT_LEN];
    size_t pos;
    int result;
} context_t;

state_t handle_start(context_t *ctx);
state_t handle_reading(context_t *ctx);
state_t handle_processing(context_t *ctx);
state_t handle_output(context_t *ctx);
state_t handle_end(context_t *ctx);

state_t (*handlers[STATE_COUNT])(context_t *) = {
    handle_start,
    handle_reading,
    handle_processing,
    handle_output,
    handle_end
};

state_t handle_start(context_t *ctx) {
    if (!ctx) return STATE_END;
    ctx->pos = 0;
    ctx->result = 0;
    *(ctx->buffer) = '\0';
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN - 1);
    return STATE_READING;
}

state_t handle_reading(context_t *ctx) {
    if (!ctx) return STATE_END;
    char *buf_ptr = ctx->buffer + ctx->pos;
    size_t remaining = MAX_INPUT_LEN - ctx->pos - 1;
    
    if (remaining == 0) return STATE_PROCESSING;
    
    int c = getchar();
    if (c == EOF || c == '\n') {
        *buf_ptr = '\0';
        return STATE_PROCESSING;
    }
    
    if (ctx->pos < MAX_INPUT_LEN - 1) {
        *buf_ptr = (char)c;
        ctx->pos++;
        return STATE_READING;
    }
    
    *buf_ptr = '\0';
    return STATE_PROCESSING;
}

state_t handle_processing(context_t *ctx) {
    if (!ctx || ctx->pos >= MAX_INPUT_LEN) return STATE_OUTPUT;
    
    char *ptr = ctx->buffer;
    int sum = 0;
    
    while (*ptr != '\0' && (size_t)(ptr - ctx->buffer) < MAX_INPUT_LEN) {
        sum += (int)(*ptr);
        ptr++;
    }
    
    ctx->result = sum;
    return STATE_OUTPUT;
}

state_t handle_output(context_t *ctx) {
    if (!ctx) return STATE_END;
    
    printf("Input: %s\n", ctx->buffer);
    printf("Character sum: %d\n", ctx->result);
    return STATE_END;
}

state_t handle_end(context_t *ctx) {
    return STATE_END;
}

int main(void) {
    context_t ctx;
    state_t current_state = STATE_START;
    
    while (current_state != STATE_END) {
        if (current_state < 0 || current_state >= STATE_COUNT) {
            break;
        }
        current_state = handlers[current_state](&ctx);
    }
    
    return 0;
}