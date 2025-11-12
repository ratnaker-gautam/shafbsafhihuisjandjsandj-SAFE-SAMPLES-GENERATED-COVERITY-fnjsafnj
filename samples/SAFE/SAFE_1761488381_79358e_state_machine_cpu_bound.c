//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

enum state {
    STATE_IDLE,
    STATE_PROCESSING,
    STATE_COMPLETED,
    STATE_ERROR,
    STATE_COUNT
};

struct context {
    enum state current_state;
    uint64_t counter;
    uint64_t target;
    char buffer[256];
    size_t buffer_pos;
};

static void state_idle(struct context *ctx) {
    if (ctx->buffer_pos >= sizeof(ctx->buffer) - 1) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    printf("Enter target value (1-1000000): ");
    if (fgets(ctx->buffer + ctx->buffer_pos, sizeof(ctx->buffer) - ctx->buffer_pos, stdin) == NULL) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    size_t len = strlen(ctx->buffer + ctx->buffer_pos);
    if (len == 0 || ctx->buffer[ctx->buffer_pos + len - 1] != '\n') {
        ctx->current_state = STATE_ERROR;
        return;
    }
    ctx->buffer[ctx->buffer_pos + len - 1] = '\0';
    char *endptr;
    unsigned long val = strtoul(ctx->buffer + ctx->buffer_pos, &endptr, 10);
    if (endptr == ctx->buffer + ctx->buffer_pos || *endptr != '\0') {
        ctx->current_state = STATE_ERROR;
        return;
    }
    if (val < 1 || val > 1000000) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    ctx->target = (uint64_t)val;
    ctx->counter = 0;
    ctx->current_state = STATE_PROCESSING;
}

static void state_processing(struct context *ctx) {
    if (ctx->counter >= ctx->target) {
        ctx->current_state = STATE_COMPLETED;
        return;
    }
    uint64_t i;
    for (i = 0; i < 1000 && ctx->counter < ctx->target; i++) {
        ctx->counter++;
    }
    if (ctx->counter >= ctx->target) {
        ctx->current_state = STATE_COMPLETED;
    }
}

static void state_completed(struct context *ctx) {
    printf("Processing completed. Counter: %lu\n", (unsigned long)ctx->counter);
    ctx->current_state = STATE_IDLE;
    ctx->buffer_pos = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

static void state_error(struct context *ctx) {
    printf("Error occurred. Resetting.\n");
    ctx->current_state = STATE_IDLE;
    ctx->buffer_pos = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->counter = 0;
    ctx->target = 0;
}

int main(void) {
    struct context ctx;
    ctx.current_state = STATE_IDLE;
    ctx.counter = 0;
    ctx.target = 0;
    ctx.buffer_pos = 0;
    memset(ctx.buffer, 0, sizeof(ctx.buffer));

    while (1) {
        switch (ctx.current_state) {
            case STATE_IDLE:
                state_idle(&ctx);
                break;
            case STATE_PROCESSING:
                state_processing(&ctx);
                break;
            case STATE_COMPLETED:
                state_completed(&ctx);
                break;
            case STATE_ERROR:
                state_error(&ctx);
                break;
            default:
                ctx.current_state = STATE_ERROR;
                break;
        }
    }

    return 0;
}