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
    if (ctx->counter >= ctx->max_iterations) {
        return STATE_COMPLETED;
    }
    if (ctx->counter % ctx->error_threshold == 0 && ctx->counter > 0) {
        return STATE_ERROR;
    }
    return STATE_PROCESSING;
}

static enum state handle_processing(struct context *ctx) {
    uint32_t next_counter;
    if (ctx->counter == UINT32_MAX) {
        return STATE_ERROR;
    }
    next_counter = ctx->counter + 1;
    if (next_counter < ctx->counter) {
        return STATE_ERROR;
    }
    ctx->counter = next_counter;
    
    for (volatile int i = 0; i < 1000; i++) {}
    
    if (ctx->counter >= ctx->max_iterations) {
        return STATE_COMPLETED;
    }
    if (ctx->counter % ctx->error_threshold == 0) {
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
        handle_completed,
        handle_error
    };
    
    uint32_t cycle_count = 0;
    const uint32_t max_cycles = iterations * 2 + 1000;
    
    while (ctx.current_state != STATE_COMPLETED && 
           ctx.current_state != STATE_ERROR &&
           cycle_count < max_cycles) {
        
        if (ctx.current_state >= STATE_COUNT) {
            ctx.current_state = STATE_ERROR;
            break;
        }
        
        state_handler handler = handlers[ctx.current_state];
        if (handler == NULL) {
            ctx.current_state = STATE_ERROR;
            break;
        }
        
        enum state next_state = handler(&ctx);
        if (next_state >= STATE_COUNT) {
            ctx.current_state = STATE_ERROR;
            break;
        }
        
        ctx.current_state = next_state;
        cycle_count++;
        
        if (cycle_count == max_cycles) {
            ctx.current_state = STATE_ERROR;
        }
    }
    
    switch (ctx.current_state) {
        case STATE_COMPLETED:
            printf("State machine completed successfully after %u iterations\n", ctx.counter);
            break;
        case STATE_ERROR:
            printf("State machine encountered error at iteration %u\n", ctx.counter);
            break;
        default:
            printf("State machine terminated unexpectedly\n");
            break;
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