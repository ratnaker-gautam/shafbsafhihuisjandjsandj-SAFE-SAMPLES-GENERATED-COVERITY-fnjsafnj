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

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 1] = {0};
    char work_buffer[MAX_INPUT_LEN + 1] = {0};
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && *(input_buffer + input_len - 1) == '\n') {
        *(input_buffer + input_len - 1) = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    memcpy(work_buffer, input_buffer, input_len + 1);
    
    context_t ctx = {
        .current = STATE_START,
        .buffer = work_buffer,
        .pos = 0,
        .len = input_len
    };
    
    typedef state_t (*state_handler_t)(context_t*);
    state_handler_t handlers[STATE_COUNT] = {
        handle_start,
        handle_reading,
        handle_processing,
        handle_final,
        handle_error
    };
    
    while (ctx.current != STATE_FINAL && ctx.current != STATE_ERROR) {
        if (ctx.current >= STATE_COUNT) {
            ctx.current = STATE_ERROR;
            break;
        }
        state_handler_t handler = *(handlers + ctx.current);
        ctx.current = handler(&ctx);
    }
    
    if (ctx.current == STATE_FINAL) {
        printf("Processing complete. Result: %s\n", ctx.buffer);
        return 0;
    } else {
        fprintf(stderr, "Processing failed\n");
        return 1;
    }
}

state_t handle_start(context_t *ctx) {
    if (ctx == NULL || ctx->buffer == NULL) {
        return STATE_ERROR;
    }
    printf("Starting processing...\n");
    ctx->pos = 0;
    return STATE_READING;
}

state_t handle_reading(context_t *ctx) {
    if (ctx == NULL || ctx->buffer == NULL) {
        return STATE_ERROR;
    }
    
    if (ctx->pos >= ctx->len) {
        return STATE_PROCESSING;
    }
    
    char *current_char = ctx->buffer + ctx->pos;
    if (*current_char >= 'a' && *current_char <= 'z') {
        *current_char = *current_char - 'a' + 'A';
    }
    
    ctx->pos++;
    return STATE_READING;
}

state_t handle_processing(context_t *ctx) {
    if (ctx == NULL || ctx->buffer == NULL) {
        return STATE_ERROR;
    }
    
    size_t valid_count = 0;
    for (size_t i = 0; i < ctx->len; i++) {
        char *c = ctx->buffer + i;
        if ((*c >= 'A' && *c <= 'Z') || (*c >= '0' && *c <= '9')) {
            if (valid_count != i) {
                *(ctx->buffer + valid_count) = *c;
            }
            valid_count++;
        }
    }
    
    if (valid_count == 0) {
        return STATE_ERROR;
    }
    
    *(ctx->buffer + valid_count) = '\0';
    ctx->len = valid_count;
    
    return STATE_FINAL;
}

state_t handle_final(context_t *ctx) {
    return STATE_FINAL;
}

state_t handle_error(context_t *ctx) {
    return STATE_ERROR;
}