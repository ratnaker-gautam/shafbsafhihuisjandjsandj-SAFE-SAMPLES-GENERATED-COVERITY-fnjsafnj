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
    printf("Processing completed. Total iterations: %u\n", ctx->counter);
    return STATE_COMPLETE;
}

static enum state handle_error(struct context *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    printf("Error occurred at iteration: %u\n", ctx->counter);
    return STATE_ERROR;
}

static void run_state_machine(uint32_t iterations, uint32_t threshold) {
    struct context ctx = {
        .current_state = STATE_IDLE,
        .counter = 0,
        .max_iterations = iterations,
        .error_threshold = threshold
    };
    
    typedef enum state (*state_handler)(struct context *);
    state_handler handlers[STATE_COUNT] = {
        handle_idle,
        handle_processing,
        handle_complete,
        handle_error
    };
    
    while (ctx.current_state != STATE_COMPLETE && 
           ctx.current_state != STATE_ERROR) {
        if (ctx.current_state >= STATE_COUNT) {
            printf("Invalid state detected\n");
            break;
        }
        
        state_handler handler = handlers[ctx.current_state];
        if (handler == NULL) {
            printf("No handler for state\n");
            break;
        }
        
        enum state next_state = handler(&ctx);
        if (next_state >= STATE_COUNT) {
            printf("Invalid next state\n");
            break;
        }
        
        ctx.current_state = next_state;
    }
}

int main(void) {
    char input_buffer[64];
    uint32_t iterations, threshold;
    
    printf("Enter maximum iterations (1-1000000): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Input error\n");
        return EXIT_FAILURE;
    }
    
    char *endptr;
    long iter_long = strtol(input_buffer, &endptr, 10);
    if (endptr == input_buffer || *endptr != '\n') {
        printf("Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    if (iter_long <= 0 || iter_long > 1000000) {
        printf("Iterations out of range\n");
        return EXIT_FAILURE;
    }
    iterations = (uint32_t)iter_long;
    
    printf("Enter error threshold (1-%u): ", iterations);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Input error\n");
        return EXIT_FAILURE;
    }
    
    long thresh_long = strtol(input_buffer, &endptr, 10);
    if (endptr == input_buffer || *endptr != '\n') {
        printf("Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    if (thresh_long <= 0 || thresh_long > (long)iterations) {
        printf("Threshold out of range\n");
        return EXIT_FAILURE;
    }
    threshold = (uint32_t)thresh_long;
    
    if (!validate_input(iterations, threshold)) {
        printf("Parameter validation failed\n");
        return EXIT_FAILURE;
    }
    
    run_state_machine(iterations, threshold);
    
    return EXIT_SUCCESS;
}