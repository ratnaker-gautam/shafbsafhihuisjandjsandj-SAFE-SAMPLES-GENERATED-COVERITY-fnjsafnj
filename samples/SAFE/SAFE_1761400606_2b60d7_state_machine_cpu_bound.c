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
    printf("State: IDLE -> PROCESSING\n");
    return STATE_PROCESSING;
}

static enum state handle_processing(struct context *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    if (ctx->counter >= ctx->max_iterations) {
        printf("State: PROCESSING -> COMPLETE\n");
        return STATE_COMPLETE;
    }
    
    uint32_t result = ctx->counter * ctx->counter;
    if (result < ctx->counter) {
        printf("State: PROCESSING -> ERROR (overflow)\n");
        return STATE_ERROR;
    }
    
    if (ctx->counter % ctx->error_threshold == (ctx->error_threshold - 1)) {
        printf("State: PROCESSING -> ERROR (threshold)\n");
        return STATE_ERROR;
    }
    
    ctx->counter++;
    return STATE_PROCESSING;
}

static enum state handle_complete(struct context *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    printf("State: COMPLETE -> IDLE\n");
    return STATE_IDLE;
}

static enum state handle_error(struct context *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    printf("State: ERROR -> IDLE\n");
    return STATE_IDLE;
}

static void run_state_machine(uint32_t iterations, uint32_t threshold) {
    struct context ctx = {
        .current_state = STATE_IDLE,
        .counter = 0,
        .max_iterations = iterations,
        .error_threshold = threshold
    };
    
    uint32_t cycle_count = 0;
    const uint32_t max_cycles = 10;
    
    while (cycle_count < max_cycles && ctx.current_state != STATE_COMPLETE) {
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
        cycle_count++;
        
        if (ctx.current_state == STATE_IDLE && cycle_count < max_cycles) {
            ctx.max_iterations = iterations;
            ctx.error_threshold = threshold;
        }
    }
    
    printf("Final state: %u\n", ctx.current_state);
    printf("Total cycles: %u\n", cycle_count);
}

int main(void) {
    char input_buffer[64];
    uint32_t iterations, threshold;
    
    printf("Enter maximum iterations (1-1000000): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return EXIT_FAILURE;
    }
    
    if (sscanf(input_buffer, "%u", &iterations) != 1) {
        fprintf(stderr, "Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter error threshold (1-%u): ", iterations);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return EXIT_FAILURE;
    }
    
    if (sscanf(input_buffer, "%u", &threshold) != 1) {
        fprintf(stderr, "Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    if (!validate_input(iterations, threshold)) {
        fprintf(stderr, "Invalid parameters\n");
        return EXIT_FAILURE;
    }
    
    run_state_machine(iterations, threshold);
    
    return EXIT_SUCCESS;
}