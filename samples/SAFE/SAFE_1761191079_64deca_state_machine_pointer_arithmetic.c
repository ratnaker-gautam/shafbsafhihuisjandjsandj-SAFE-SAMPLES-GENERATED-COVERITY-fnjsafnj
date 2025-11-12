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
    char *buffer;
    size_t pos;
    size_t len;
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
    printf("Starting state machine\n");
    ctx->pos = 0;
    return STATE_READING;
}

state_t handle_reading(context_t *ctx) {
    if (ctx == NULL || ctx->buffer == NULL) return STATE_ERROR;
    
    printf("Enter text (max %zu chars): ", ctx->len);
    if (fgets(ctx->buffer, (int)(ctx->len + 1), stdin) == NULL) {
        return STATE_ERROR;
    }
    
    size_t input_len = strlen(ctx->buffer);
    if (input_len > 0 && *(ctx->buffer + input_len - 1) == '\n') {
        *(ctx->buffer + input_len - 1) = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("Empty input\n");
        return STATE_ERROR;
    }
    
    ctx->pos = 0;
    return STATE_PROCESSING;
}

state_t handle_processing(context_t *ctx) {
    if (ctx == NULL || ctx->buffer == NULL) return STATE_ERROR;
    
    char *current = ctx->buffer + ctx->pos;
    size_t remaining = strlen(current);
    
    if (remaining == 0) {
        return STATE_FINISHED;
    }
    
    printf("Processing: ");
    char *end = current + (remaining > 10 ? 10 : remaining);
    while (current < end) {
        if (*current >= 'a' && *current <= 'z') {
            *current = *current - 'a' + 'A';
        }
        putchar(*current);
        current++;
        ctx->pos++;
    }
    printf("\n");
    
    return STATE_PROCESSING;
}

state_t handle_finished(context_t *ctx) {
    if (ctx == NULL || ctx->buffer == NULL) return STATE_ERROR;
    
    printf("Final result: %s\n", ctx->buffer);
    return STATE_FINISHED;
}

state_t handle_error(context_t *ctx) {
    if (ctx != NULL) {
        printf("Error occurred in state machine\n");
    }
    return STATE_ERROR;
}

int main(void) {
    context_t ctx;
    ctx.current = STATE_START;
    ctx.len = MAX_INPUT_LEN;
    ctx.buffer = malloc(ctx.len + 1);
    
    if (ctx.buffer == NULL) {
        return EXIT_FAILURE;
    }
    
    memset(ctx.buffer, 0, ctx.len + 1);
    
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
    
    free(ctx.buffer);
    
    return ctx.current == STATE_FINISHED ? EXIT_SUCCESS : EXIT_FAILURE;
}