//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define MAX_INPUT_LEN 100
#define STATE_COUNT 5

typedef enum {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_FINALIZING,
    STATE_DONE
} state_t;

typedef struct {
    state_t current_state;
    char input_buffer[MAX_INPUT_LEN + 1];
    size_t input_len;
    uint32_t checksum;
} context_t;

static state_t handle_start(context_t *ctx) {
    printf("Enter text to process (max %d chars): ", MAX_INPUT_LEN);
    if (fgets(ctx->input_buffer, sizeof(ctx->input_buffer), stdin) == NULL) {
        return STATE_DONE;
    }
    ctx->input_len = strlen(ctx->input_buffer);
    if (ctx->input_len > 0 && ctx->input_buffer[ctx->input_len - 1] == '\n') {
        ctx->input_buffer[--ctx->input_len] = '\0';
    }
    if (ctx->input_len == 0) {
        return STATE_DONE;
    }
    return STATE_READING;
}

static state_t handle_reading(context_t *ctx) {
    ctx->checksum = 0;
    for (size_t i = 0; i < ctx->input_len; i++) {
        ctx->checksum = (ctx->checksum * 31) + (uint8_t)ctx->input_buffer[i];
    }
    return STATE_PROCESSING;
}

static state_t handle_processing(context_t *ctx) {
    for (size_t i = 0; i < ctx->input_len; i++) {
        for (int j = 0; j < 1000; j++) {
            ctx->input_buffer[i] ^= (j & 0xFF);
        }
    }
    return STATE_FINALIZING;
}

static state_t handle_finalizing(context_t *ctx) {
    uint32_t final_checksum = 0;
    for (size_t i = 0; i < ctx->input_len; i++) {
        final_checksum = (final_checksum * 31) + (uint8_t)ctx->input_buffer[i];
    }
    printf("Initial checksum: %u\n", ctx->checksum);
    printf("Final checksum: %u\n", final_checksum);
    printf("Processing complete.\n");
    return STATE_DONE;
}

static state_t handle_done(context_t *ctx) {
    return STATE_DONE;
}

typedef state_t (*state_handler_t)(context_t*);

static const state_handler_t state_handlers[STATE_COUNT] = {
    handle_start,
    handle_reading,
    handle_processing,
    handle_finalizing,
    handle_done
};

int main(void) {
    context_t ctx = {0};
    ctx.current_state = STATE_START;
    
    while (ctx.current_state != STATE_DONE) {
        if (ctx.current_state >= STATE_COUNT) {
            break;
        }
        state_handler_t handler = state_handlers[ctx.current_state];
        if (handler == NULL) {
            break;
        }
        state_t next_state = handler(&ctx);
        ctx.current_state = next_state;
    }
    
    return 0;
}