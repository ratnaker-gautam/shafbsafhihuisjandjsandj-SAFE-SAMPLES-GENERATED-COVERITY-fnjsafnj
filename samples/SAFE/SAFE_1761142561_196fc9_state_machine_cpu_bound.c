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
    STATE_WRITING,
    STATE_DONE
} state_t;

typedef struct {
    state_t current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_len;
    uint32_t checksum;
} context_t;

static uint32_t compute_checksum(const char *data, size_t len) {
    uint32_t sum = 0;
    for (size_t i = 0; i < len; i++) {
        sum = (sum * 31) + (uint8_t)data[i];
    }
    return sum;
}

static state_t handle_start(context_t *ctx) {
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN - 1);
    if (fgets(ctx->buffer, MAX_INPUT_LEN, stdin) == NULL) {
        return STATE_DONE;
    }
    ctx->buffer_len = strlen(ctx->buffer);
    if (ctx->buffer_len > 0 && ctx->buffer[ctx->buffer_len - 1] == '\n') {
        ctx->buffer[--ctx->buffer_len] = '\0';
    }
    if (ctx->buffer_len == 0) {
        return STATE_DONE;
    }
    return STATE_READING;
}

static state_t handle_reading(context_t *ctx) {
    ctx->checksum = compute_checksum(ctx->buffer, ctx->buffer_len);
    return STATE_PROCESSING;
}

static state_t handle_processing(context_t *ctx) {
    for (size_t i = 0; i < ctx->buffer_len; i++) {
        for (int j = 0; j < 1000; j++) {
            ctx->buffer[i] ^= (j & 0xFF);
        }
    }
    return STATE_WRITING;
}

static state_t handle_writing(context_t *ctx) {
    printf("Processed data: ");
    for (size_t i = 0; i < ctx->buffer_len; i++) {
        printf("%02X ", (uint8_t)ctx->buffer[i]);
    }
    printf("\nChecksum: %u\n", ctx->checksum);
    return STATE_DONE;
}

static state_t handle_done(context_t *ctx) {
    return STATE_DONE;
}

static state_t (*state_handlers[STATE_COUNT])(context_t *) = {
    handle_start,
    handle_reading,
    handle_processing,
    handle_writing,
    handle_done
};

int main(void) {
    context_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    ctx.current_state = STATE_START;
    
    while (ctx.current_state != STATE_DONE) {
        if (ctx.current_state >= STATE_COUNT) {
            break;
        }
        state_t next_state = state_handlers[ctx.current_state](&ctx);
        ctx.current_state = next_state;
    }
    
    return 0;
}