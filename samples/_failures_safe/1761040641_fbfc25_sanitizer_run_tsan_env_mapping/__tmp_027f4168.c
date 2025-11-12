//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: state_machine
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
    
    uint32_t temp = ctx->counter;
    for (int i = 0; i < 100; i++) {
        temp = (temp * 1103515245 + 12345) % 2147483647;
    }
    ctx->counter = temp % ctx->max_iterations;
    
    if (ctx->counter >= ctx->error_threshold) {
        return STATE_ERROR;
    }
    
    ctx->counter++;
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

static void run_state_machine(struct context *ctx) {
    if (ctx == NULL) return;
    
    typedef enum state (*state_handler)(struct context*);
    state_handler handlers[STATE_COUNT] = {
        handle_idle,
        handle_processing,
        handle_complete,
        handle_error
    };
    
    while (1) {
        if (ctx->current_state >= STATE_COUNT) {
            ctx->current_state = STATE_ERROR;
        }
        
        enum state next_state = handlers[ctx->current_state](ctx);
        
        if (next_state == ctx->current_state && 
            next_state != STATE_PROCESSING) {
            break;
        }
        
        ctx->current_state = next_state;
    }
}

int main(void) {
    uint32_t iterations, threshold;
    
    printf("Enter maximum iterations (1-1000000): ");
    if (scanf("%u", &iterations) != 1) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter error threshold (1-%u): ", iterations);
    if (scanf("%u", &threshold) != 1) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    if (!validate_input(iterations, threshold)) {
        fprintf(stderr, "Invalid parameters\n");
        return EXIT_FAILURE;
    }
    
    struct context ctx = {
        .current_state = STATE_IDLE,
        .counter = 0,
        .max_iterations = iterations,
        .error_threshold = threshold
    };
    
    run_state_machine(&ctx);
    
    switch (ctx.current_state) {
        case STATE_COMPLETE:
            printf("Processing completed successfully\n");
            break;
        case STATE_ERROR:
            printf("Processing failed at iteration %u\n", ctx.counter);
            break;
        default:
            printf("Unexpected state: %d\n", ctx.current_state);
            return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}