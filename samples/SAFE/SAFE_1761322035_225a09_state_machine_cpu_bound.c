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
    uint32_t i;
    uint64_t sum = 0;
    for (i = 0; i < 1000; i++) {
        if (i > 0 && sum > UINT64_MAX - (uint64_t)i) {
            return STATE_ERROR;
        }
        sum += (uint64_t)i;
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
    printf("Error occurred at iteration %u\n", ctx->counter);
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

int main(int argc, char *argv[]) {
    struct context ctx;
    ctx.current_state = STATE_IDLE;
    ctx.max_iterations = 10000;
    
    if (argc > 1) {
        char *endptr;
        long val = strtol(argv[1], &endptr, 10);
        if (endptr != argv[1] && *endptr == '\0' && val > 0 && val <= 1000000) {
            ctx.max_iterations = (uint32_t)val;
        }
    }
    
    int cycle_count = 0;
    while (ctx.current_state != STATE_COMPLETE && ctx.current_state != STATE_ERROR) {
        if (cycle_count++ > 1000000) {
            break;
        }
        
        enum state prev_state = ctx.current_state;
        enter_funcs[ctx.current_state](&ctx);
        ctx.current_state = run_funcs[prev_state](&ctx);
        
        if (ctx.current_state >= STATE_COUNT) {
            ctx.current_state = STATE_ERROR;
        }
    }
    
    if (ctx.current_state == STATE_COMPLETE) {
        printf("State machine finished successfully.\n");
    } else {
        printf("State machine terminated with error.\n");
    }
    
    return ctx.current_state == STATE_COMPLETE ? 0 : 1;
}