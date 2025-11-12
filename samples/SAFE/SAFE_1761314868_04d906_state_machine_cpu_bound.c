//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

enum state {
    STATE_IDLE,
    STATE_PROCESSING,
    STATE_COMPLETE,
    STATE_ERROR,
    STATE_COUNT
};

struct context {
    enum state current_state;
    uint32_t counter;
    uint32_t max_iterations;
    char buffer[256];
    size_t buffer_pos;
};

static void state_idle_enter(struct context *ctx) {
    ctx->counter = 0;
    ctx->buffer_pos = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

static enum state state_idle_run(struct context *ctx) {
    if (ctx->buffer_pos >= sizeof(ctx->buffer) - 1) {
        return STATE_ERROR;
    }
    ctx->buffer[ctx->buffer_pos] = 'A' + (ctx->counter % 26);
    ctx->buffer_pos++;
    ctx->counter++;
    if (ctx->counter >= 10) {
        return STATE_PROCESSING;
    }
    return STATE_IDLE;
}

static void state_processing_enter(struct context *ctx) {
    ctx->counter = 0;
}

static enum state state_processing_run(struct context *ctx) {
    if (ctx->counter >= ctx->max_iterations) {
        return STATE_COMPLETE;
    }
    uint32_t sum = 0;
    for (uint32_t i = 0; i < 1000; i++) {
        sum += (i * i) % 100;
    }
    ctx->counter++;
    return STATE_PROCESSING;
}

static void state_complete_enter(struct context *ctx) {
    printf("Processing complete. Result: %s\n", ctx->buffer);
}

static enum state state_complete_run(struct context *ctx) {
    return STATE_COMPLETE;
}

static void state_error_enter(struct context *ctx) {
    printf("Error: Buffer overflow detected\n");
}

static enum state state_error_run(struct context *ctx) {
    return STATE_ERROR;
}

typedef void (*state_enter_func)(struct context *);
typedef enum state (*state_run_func)(struct context *);

static const state_enter_func enter_funcs[STATE_COUNT] = {
    state_idle_enter,
    state_processing_enter,
    state_complete_enter,
    state_error_enter
};

static const state_run_func run_funcs[STATE_COUNT] = {
    state_idle_run,
    state_processing_run,
    state_complete_run,
    state_error_run
};

static void run_state_machine(struct context *ctx) {
    enum state next_state = ctx->current_state;
    do {
        if (ctx->current_state >= STATE_COUNT) {
            break;
        }
        enter_funcs[ctx->current_state](ctx);
        next_state = run_funcs[ctx->current_state](ctx);
        ctx->current_state = next_state;
    } while (ctx->current_state != STATE_COMPLETE && ctx->current_state != STATE_ERROR);
}

int main(void) {
    struct context ctx;
    ctx.current_state = STATE_IDLE;
    ctx.max_iterations = 10000;
    
    if (ctx.max_iterations == 0 || ctx.max_iterations > 1000000) {
        fprintf(stderr, "Invalid iteration count\n");
        return EXIT_FAILURE;
    }
    
    run_state_machine(&ctx);
    
    if (ctx.current_state == STATE_ERROR) {
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}