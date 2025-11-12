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
    char *input;
    size_t pos;
    size_t len;
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
    ctx->result = 0;
    ctx->pos = 0;
    printf("Enter a string to process: ");
    return STATE_READING;
}

state_t handle_reading(context_t *ctx) {
    char buffer[MAX_INPUT_LEN];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return STATE_END;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && *(buffer + len - 1) == '\n') {
        *(buffer + len - 1) = '\0';
        len--;
    }
    
    if (len >= MAX_INPUT_LEN - 1) {
        return STATE_END;
    }
    
    ctx->input = malloc(len + 1);
    if (ctx->input == NULL) {
        return STATE_END;
    }
    
    memcpy(ctx->input, buffer, len + 1);
    ctx->len = len;
    return STATE_PROCESSING;
}

state_t handle_processing(context_t *ctx) {
    if (ctx->input == NULL || ctx->len == 0) {
        return STATE_END;
    }
    
    char *ptr = ctx->input;
    char *end = ptr + ctx->len;
    int sum = 0;
    
    while (ptr < end) {
        if (*ptr >= '0' && *ptr <= '9') {
            sum += (*ptr - '0');
        }
        ptr++;
    }
    
    ctx->result = sum;
    return STATE_OUTPUT;
}

state_t handle_output(context_t *ctx) {
    printf("Sum of digits in input: %d\n", ctx->result);
    return STATE_END;
}

state_t handle_end(context_t *ctx) {
    if (ctx->input != NULL) {
        free(ctx->input);
        ctx->input = NULL;
    }
    return STATE_END;
}

int main(void) {
    context_t ctx = {STATE_START, NULL, 0, 0, 0};
    
    while (ctx.current != STATE_END) {
        if (ctx.current >= 0 && ctx.current < STATE_COUNT) {
            state_t next = handlers[ctx.current](&ctx);
            ctx.current = next;
        } else {
            break;
        }
    }
    
    if (ctx.input != NULL) {
        free(ctx.input);
    }
    
    return 0;
}