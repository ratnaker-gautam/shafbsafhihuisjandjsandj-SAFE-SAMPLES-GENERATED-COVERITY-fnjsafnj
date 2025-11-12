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

static enum state handle_idle(struct context *ctx, uint32_t input) {
    if (input > ctx->max_iterations) {
        return STATE_ERROR;
    }
    ctx->counter = 0;
    return STATE_PROCESSING;
}

static enum state handle_processing(struct context *ctx) {
    if (ctx->counter >= ctx->max_iterations) {
        return STATE_COMPLETED;
    }
    
    uint32_t temp = ctx->counter;
    for (int i = 0; i < 100; i++) {
        temp = (temp * 1103515245 + 12345) & 0x7FFFFFFF;
    }
    ctx->counter = temp % (ctx->max_iterations + 1);
    
    if (ctx->counter % ctx->error_threshold == 0 && ctx->counter != 0) {
        return STATE_ERROR;
    }
    
    return STATE_PROCESSING;
}

static enum state handle_completed(struct context *ctx) {
    return STATE_COMPLETED;
}

static enum state handle_error(struct context *ctx) {
    return STATE_ERROR;
}

static void run_state_machine(uint32_t iterations, uint32_t threshold) {
    struct context ctx = {
        .current_state = STATE_IDLE,
        .counter = 0,
        .max_iterations = iterations,
        .error_threshold = threshold
    };
    
    typedef enum state (*state_handler)(struct context *, uint32_t);
    state_handler handlers[STATE_COUNT] = {
        handle_idle,
        (state_handler)handle_processing,
        (state_handler)handle_completed,
        (state_handler)handle_error
    };
    
    uint32_t step_count = 0;
    uint32_t input_value = iterations / 2;
    
    while (ctx.current_state != STATE_COMPLETED && 
           ctx.current_state != STATE_ERROR &&
           step_count < iterations * 2) {
        
        enum state next_state;
        if (ctx.current_state == STATE_IDLE) {
            next_state = handlers[ctx.current_state](&ctx, input_value);
        } else {
            next_state = handlers[ctx.current_state](&ctx, 0);
        }
        
        if (next_state < STATE_IDLE || next_state >= STATE_COUNT) {
            ctx.current_state = STATE_ERROR;
        } else {
            ctx.current_state = next_state;
        }
        
        step_count++;
        
        if (step_count >= iterations * 2) {
            ctx.current_state = STATE_ERROR;
        }
    }
    
    if (ctx.current_state == STATE_COMPLETED) {
        printf("State machine completed successfully after %u steps\n", step_count);
        printf("Final counter value: %u\n", ctx.counter);
    } else {
        printf("State machine terminated with error after %u steps\n", step_count);
        printf("Final counter value: %u\n", ctx.counter);
    }
}

int main(void) {
    char input_buffer[64];
    uint32_t iterations, threshold;
    
    printf("Enter maximum iterations (1-1000000): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    if (sscanf(input_buffer, "%u", &iterations) != 1) {
        fprintf(stderr, "Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter error threshold (1-%u): ", iterations);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
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