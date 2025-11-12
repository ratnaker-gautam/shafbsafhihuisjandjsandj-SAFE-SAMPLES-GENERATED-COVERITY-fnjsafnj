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
        return STATE_COMPLETED;
    }
    
    uint32_t result = ctx->counter * ctx->counter;
    if (result < ctx->counter) {
        return STATE_ERROR;
    }
    
    ctx->counter++;
    
    if (ctx->counter % ctx->error_threshold == 0) {
        uint32_t check = UINT_MAX / ctx->counter;
        if (check == 0) {
            return STATE_ERROR;
        }
    }
    
    return STATE_PROCESSING;
}

static enum state handle_completed(struct context *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    return STATE_COMPLETED;
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
        enum state next_state = ctx.current_state;
        
        switch (ctx.current_state) {
            case STATE_IDLE:
                next_state = handle_idle(&ctx);
                break;
            case STATE_PROCESSING:
                next_state = handle_processing(&ctx);
                break;
            case STATE_COMPLETED:
                next_state = handle_completed(&ctx);
                break;
            case STATE_ERROR:
                next_state = handle_error(&ctx);
                break;
            default:
                next_state = STATE_ERROR;
                break;
        }
        
        if (next_state >= STATE_COUNT) {
            next_state = STATE_ERROR;
        }
        
        ctx.current_state = next_state;
        cycle_count++;
        
        if (ctx.current_state == STATE_COMPLETED || ctx.current_state == STATE_ERROR) {
            break;
        }
    }
    
    const char *state_names[] = {
        "IDLE", "PROCESSING", "COMPLETED", "ERROR"
    };
    
    const char *result_name = "UNKNOWN";
    if (ctx.current_state < STATE_COUNT) {
        result_name = state_names[ctx.current_state];
    }
    
    printf("Final state: %s\n", result_name);
    printf("Iterations completed: %u\n", ctx.counter);
    printf("Total cycles: %u\n", cycle_count);
}

int main(void) {
    char input_buffer[64];
    uint32_t iterations = 0;
    uint32_t threshold = 0;
    
    printf("Enter maximum iterations (1-1000000): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    char *endptr;
    long iter_long = strtol(input_buffer, &endptr, 10);
    if (endptr == input_buffer || *endptr != '\n' && *endptr != '\0') {
        printf("Invalid input format\n");
        return 1;
    }
    
    if (iter_long < 1 || iter_long > 1000000) {
        printf("Iterations out of range\n");
        return 1;
    }
    iterations = (uint32_t)iter_long;
    
    printf("Enter error threshold (1-%u): ", iterations);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    long thresh_long = strtol(input_buffer, &endptr, 10);
    if (endptr == input_buffer || *endptr != '\n' && *endptr != '\0') {
        printf("Invalid input format\n");
        return 1;
    }
    
    if (thresh_long < 1 || thresh_long > (long)iterations) {
        printf("Threshold out of range\n");
        return 1;
    }
    threshold = (uint32_t)thresh_long;
    
    if (!validate_input(iterations, threshold)) {
        printf("Validation failed\n");
        return 1;
    }
    
    run_state_machine(iterations, threshold);
    
    return 0;
}