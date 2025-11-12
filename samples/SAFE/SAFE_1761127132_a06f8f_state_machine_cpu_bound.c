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
    return STATE_PROCESSING;
}

static enum state handle_processing(struct context *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    if (ctx->counter >= ctx->max_iterations) {
        return STATE_COMPLETE;
    }
    
    uint32_t old_counter = ctx->counter;
    ctx->counter++;
    
    if (ctx->counter <= old_counter) {
        return STATE_ERROR;
    }
    
    if (ctx->counter % ctx->error_threshold == 0) {
        return STATE_ERROR;
    }
    
    return STATE_PROCESSING;
}

static enum state handle_complete(struct context *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    return STATE_COMPLETE;
}

static enum state handle_error(struct context *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    return STATE_ERROR;
}

static void run_state_machine(uint32_t iterations, uint32_t threshold) {
    struct context ctx;
    ctx.current_state = STATE_IDLE;
    ctx.max_iterations = iterations;
    ctx.error_threshold = threshold;
    
    uint32_t cycle_count = 0;
    const uint32_t max_cycles = iterations * 2;
    
    while (cycle_count < max_cycles) {
        cycle_count++;
        
        switch (ctx.current_state) {
            case STATE_IDLE:
                ctx.current_state = handle_idle(&ctx);
                break;
            case STATE_PROCESSING:
                ctx.current_state = handle_processing(&ctx);
                break;
            case STATE_COMPLETE:
                ctx.current_state = handle_complete(&ctx);
                break;
            case STATE_ERROR:
                ctx.current_state = handle_error(&ctx);
                break;
            default:
                ctx.current_state = STATE_ERROR;
                break;
        }
        
        if (ctx.current_state == STATE_COMPLETE || ctx.current_state == STATE_ERROR) {
            break;
        }
    }
    
    if (ctx.current_state == STATE_COMPLETE) {
        printf("Processing completed successfully after %u iterations\n", ctx.counter);
    } else if (ctx.current_state == STATE_ERROR) {
        printf("Processing failed at iteration %u\n", ctx.counter);
    } else {
        printf("Processing timed out after %u cycles\n", cycle_count);
    }
}

int main(void) {
    char input_buffer[64];
    uint32_t iterations, threshold;
    
    printf("Enter maximum iterations (1-1000000): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    if (sscanf(input_buffer, "%u", &iterations) != 1) {
        printf("Invalid input format\n");
        return 1;
    }
    
    printf("Enter error threshold (1-%u): ", iterations);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    if (sscanf(input_buffer, "%u", &threshold) != 1) {
        printf("Invalid input format\n");
        return 1;
    }
    
    if (!validate_input(iterations, threshold)) {
        printf("Invalid parameters\n");
        return 1;
    }
    
    run_state_machine(iterations, threshold);
    
    return 0;
}