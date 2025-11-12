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
    
    uint32_t step_count = 0;
    const uint32_t max_steps = iterations * 2 + 100;
    
    while (step_count < max_steps) {
        enum state next_state;
        
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
            ctx.current_state = STATE_ERROR;
        } else {
            ctx.current_state = next_state;
        }
        
        step_count++;
        
        if (ctx.current_state == STATE_COMPLETED || ctx.current_state == STATE_ERROR) {
            break;
        }
    }
    
    const char *state_names[] = {"IDLE", "PROCESSING", "COMPLETED", "ERROR"};
    int state_index = ctx.current_state;
    if (state_index < 0 || state_index >= STATE_COUNT) {
        state_index = STATE_ERROR;
    }
    
    printf("Final state: %s\n", state_names[state_index]);
    printf("Iterations processed: %u\n", ctx.counter);
    printf("Total steps: %u\n", step_count);
}

int main(void) {
    char input_buffer[64];
    uint32_t iterations = 0;
    uint32_t threshold = 0;
    
    printf("Enter maximum iterations (1-1000000): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    char *endptr;
    long iter_long = strtol(input_buffer, &endptr, 10);
    if (endptr == input_buffer || *endptr != '\n') {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    if (iter_long < 1 || iter_long > 1000000) {
        fprintf(stderr, "Iterations out of range\n");
        return 1;
    }
    iterations = (uint32_t)iter_long;
    
    printf("Enter error threshold (1-%u): ", iterations);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    long thresh_long = strtol(input_buffer, &endptr, 10);
    if (endptr == input_buffer || *endptr != '\n') {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    if (thresh_long < 1 || thresh_long > (long)iterations) {
        fprintf(stderr, "Threshold out of range\n");
        return 1;
    }
    threshold = (uint32_t)thresh_long;
    
    if (!validate_input(iterations, threshold)) {
        fprintf(stderr, "Validation failed\n");
        return 1;
    }
    
    run_state_machine(iterations, threshold);
    
    return 0;
}