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
    STATE_FINAL,
    STATE_ERROR
} state_t;

typedef struct {
    state_t current;
    char *buffer;
    size_t pos;
    size_t len;
} context_t;

state_t handle_start(context_t *ctx);
state_t handle_reading(context_t *ctx);
state_t handle_processing(context_t *ctx);
state_t handle_final(context_t *ctx);
state_t handle_error(context_t *ctx);

state_t (*handlers[STATE_COUNT])(context_t *) = {
    handle_start,
    handle_reading,
    handle_processing,
    handle_final,
    handle_error
};

state_t handle_start(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    ctx->pos = 0;
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN - 1);
    return STATE_READING;
}

state_t handle_reading(context_t *ctx) {
    if (ctx == NULL || ctx->buffer == NULL) return STATE_ERROR;
    
    char input[MAX_INPUT_LEN];
    if (fgets(input, sizeof(input), stdin) == NULL) return STATE_ERROR;
    
    size_t input_len = strlen(input);
    if (input_len > 0 && *(input + input_len - 1) == '\n') {
        *(input + input_len - 1) = '\0';
        input_len--;
    }
    
    if (input_len >= ctx->len) return STATE_ERROR;
    
    for (size_t i = 0; i < input_len; i++) {
        if (ctx->pos >= ctx->len - 1) break;
        *(ctx->buffer + ctx->pos) = *(input + i);
        ctx->pos++;
    }
    *(ctx->buffer + ctx->pos) = '\0';
    
    return STATE_PROCESSING;
}

state_t handle_processing(context_t *ctx) {
    if (ctx == NULL || ctx->buffer == NULL) return STATE_ERROR;
    
    if (ctx->pos == 0) return STATE_ERROR;
    
    char *start = ctx->buffer;
    char *end = ctx->buffer + ctx->pos - 1;
    
    while (start < end) {
        char temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }
    
    return STATE_FINAL;
}

state_t handle_final(context_t *ctx) {
    if (ctx == NULL || ctx->buffer == NULL) return STATE_ERROR;
    
    printf("Reversed: %s\n", ctx->buffer);
    return STATE_START;
}

state_t handle_error(context_t *ctx) {
    if (ctx != NULL) {
        printf("Error occurred\n");
    }
    return STATE_ERROR;
}

int main(void) {
    char buffer[MAX_INPUT_LEN];
    context_t ctx;
    
    ctx.current = STATE_START;
    ctx.buffer = buffer;
    ctx.pos = 0;
    ctx.len = sizeof(buffer);
    
    while (1) {
        if (ctx.current >= STATE_COUNT) {
            ctx.current = STATE_ERROR;
        }
        
        state_t next_state = handlers[ctx.current](&ctx);
        
        if (next_state == STATE_ERROR && ctx.current == STATE_ERROR) {
            break;
        }
        
        ctx.current = next_state;
        
        if (ctx.current == STATE_START) {
            ctx.pos = 0;
            *(ctx.buffer) = '\0';
        }
    }
    
    return 0;
}