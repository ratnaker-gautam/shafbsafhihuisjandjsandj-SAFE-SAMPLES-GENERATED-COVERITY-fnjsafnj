//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: state_machine
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
    uint32_t counter;
    uint32_t max_iterations;
    uint32_t error_threshold;
};

static int validate_context(const struct context *ctx) {
    if (ctx == NULL) return 0;
    if (ctx->current_state >= STATE_COUNT) return 0;
    if (ctx->max_iterations > 1000000) return 0;
    if (ctx->error_threshold > 1000000) return 0;
    return 1;
}

static enum state handle_idle(struct context *ctx) {
    if (ctx->counter >= ctx->max_iterations) {
        return STATE_COMPLETED;
    }
    if (ctx->counter % ctx->error_threshold == 0 && ctx->counter > 0) {
        return STATE_ERROR;
    }
    return STATE_PROCESSING;
}

static enum state handle_processing(struct context *ctx) {
    uint32_t old_counter = ctx->counter;
    if (ctx->counter < UINT32_MAX - 1) {
        ctx->counter++;
    } else {
        return STATE_ERROR;
    }
    
    for (int i = 0; i < 1000; i++) {
        ctx->counter = ctx->counter ^ (ctx->counter << 1);
        ctx->counter = ctx->counter ^ (ctx->counter >> 1);
    }
    
    if (ctx->counter < old_counter) {
        return STATE_ERROR;
    }
    
    return STATE_IDLE;
}

static enum state handle_completed(struct context *ctx) {
    return STATE_COMPLETED;
}

static enum state handle_error(struct context *ctx) {
    return STATE_ERROR;
}

static enum state process_state(struct context *ctx) {
    if (!validate_context(ctx)) return STATE_ERROR;
    
    switch (ctx->current_state) {
        case STATE_IDLE:
            return handle_idle(ctx);
        case STATE_PROCESSING:
            return handle_processing(ctx);
        case STATE_COMPLETED:
            return handle_completed(ctx);
        case STATE_ERROR:
            return handle_error(ctx);
        default:
            return STATE_ERROR;
    }
}

static void run_state_machine(struct context *ctx) {
    int cycle_count = 0;
    const int max_cycles = 100000;
    
    while (cycle_count < max_cycles) {
        enum state next_state = process_state(ctx);
        
        if (next_state == ctx->current_state && 
            (next_state == STATE_COMPLETED || next_state == STATE_ERROR)) {
            break;
        }
        
        ctx->current_state = next_state;
        cycle_count++;
        
        if (cycle_count % 10000 == 0) {
            printf("Cycle: %d, State: %d, Counter: %u\n", 
                   cycle_count, ctx->current_state, ctx->counter);
        }
    }
}

int main(void) {
    struct context ctx;
    
    ctx.current_state = STATE_IDLE;
    ctx.counter = 0;
    ctx.max_iterations = 50000;
    ctx.error_threshold = 17;
    
    if (!validate_context(&ctx)) {
        fprintf(stderr, "Invalid initial context\n");
        return EXIT_FAILURE;
    }
    
    printf("Starting state machine\n");
    printf("Max iterations: %u, Error threshold: %u\n", 
           ctx.max_iterations, ctx.error_threshold);
    
    run_state_machine(&ctx);
    
    printf("Final state: %d, Final counter: %u\n", 
           ctx.current_state, ctx.counter);
    
    if (ctx.current_state == STATE_COMPLETED) {
        printf("State machine completed successfully\n");
        return EXIT_SUCCESS;
    } else {
        printf("State machine terminated with error\n");
        return EXIT_FAILURE;
    }
}