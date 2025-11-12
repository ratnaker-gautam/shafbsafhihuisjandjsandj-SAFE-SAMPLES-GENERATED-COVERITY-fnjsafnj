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
    uint32_t error_threshold;
};

static int validate_input(uint32_t iterations, uint32_t threshold) {
    if (iterations == 0 || iterations > 1000000) return 0;
    if (threshold == 0 || threshold > iterations) return 0;
    return 1;
}

static enum state handle_idle(struct context *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    ctx->counter = 0;
    printf("Starting processing...\n");
    return STATE_PROCESSING;
}

static enum state handle_processing(struct context *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    if (ctx->counter >= ctx->max_iterations) {
        printf("Processing complete after %u iterations\n", ctx->counter);
        return STATE_COMPLETE;
    }
    
    uint32_t old_counter = ctx->counter;
    if (UINT32_MAX - old_counter < 1) {
        return STATE_ERROR;
    }
    ctx->counter++;
    
    if (ctx->counter % ctx->error_threshold == 0) {
        uint32_t result = ctx->counter * ctx->counter;
        if (result / ctx->counter != ctx->counter) {
            printf("Overflow detected at iteration %u\n", ctx->counter);
            return STATE_ERROR;
        }
    }
    
    if (ctx->counter % 10000 == 0) {
        printf("Progress: %u/%u\n", ctx->counter, ctx->max_iterations);
    }
    
    return STATE_PROCESSING;
}

static enum state handle_complete(struct context *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    printf("Final count: %u\n", ctx->counter);
    return STATE_COMPLETE;
}

static enum state handle_error(struct context *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    printf("Error state reached at iteration %u\n", ctx->counter);
    return STATE_ERROR;
}

static void run_state_machine(uint32_t iterations, uint32_t threshold) {
    struct context ctx;
    ctx.current_state = STATE_IDLE;
    ctx.max_iterations = iterations;
    ctx.error_threshold = threshold;
    
    int cycle_count = 0;
    const int max_cycles = 1000000;
    
    while (ctx.current_state != STATE_COMPLETE && 
           ctx.current_state != STATE_ERROR &&
           cycle_count < max_cycles) {
        
        cycle_count++;
        if (cycle_count >= max_cycles) {
            printf("Cycle limit exceeded\n");
            break;
        }
        
        enum state next_state;
        switch (ctx.current_state) {
            case STATE_IDLE:
                next_state = handle_idle(&ctx);
                break;
            case STATE_PROCESSING:
                next_state = handle_processing(&ctx);
                break;
            case STATE_COMPLETE:
                next_state = handle_complete(&ctx);
                break;
            case STATE_ERROR:
                next_state = handle_error(&ctx);
                break;
            default:
                next_state = STATE_ERROR;
                break;
        }
        
        ctx.current_state = next_state;
    }
    
    if (cycle_count >= max_cycles) {
        printf("State machine terminated due to cycle limit\n");
    }
}

int main(void) {
    uint32_t iterations = 50000;
    uint32_t threshold = 1000;
    
    if (!validate_input(iterations, threshold)) {
        fprintf(stderr, "Invalid input parameters\n");
        return EXIT_FAILURE;
    }
    
    printf("State Machine Demo\n");
    printf("Iterations: %u, Error threshold: %u\n", iterations, threshold);
    
    run_state_machine(iterations, threshold);
    
    printf("Program completed\n");
    return EXIT_SUCCESS;
}