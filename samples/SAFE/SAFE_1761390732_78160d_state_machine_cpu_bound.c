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

static void state_idle(struct context *ctx) {
    if (ctx == NULL) return;
    ctx->counter = 0;
    ctx->current_state = STATE_PROCESSING;
}

static void state_processing(struct context *ctx) {
    if (ctx == NULL) return;
    
    if (ctx->counter >= ctx->max_iterations) {
        ctx->current_state = STATE_COMPLETED;
        return;
    }
    
    uint32_t result = ctx->counter * ctx->counter;
    if (result < ctx->counter) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    if (ctx->counter % ctx->error_threshold == (ctx->error_threshold - 1)) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    ctx->counter++;
}

static void state_completed(struct context *ctx) {
    if (ctx == NULL) return;
}

static void state_error(struct context *ctx) {
    if (ctx == NULL) return;
}

static void run_state_machine(struct context *ctx) {
    if (ctx == NULL) return;
    
    while (ctx->current_state != STATE_COMPLETED && 
           ctx->current_state != STATE_ERROR) {
        switch (ctx->current_state) {
            case STATE_IDLE:
                state_idle(ctx);
                break;
            case STATE_PROCESSING:
                state_processing(ctx);
                break;
            case STATE_COMPLETED:
            case STATE_ERROR:
                break;
            default:
                ctx->current_state = STATE_ERROR;
                break;
        }
    }
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
    
    struct context ctx = {
        .current_state = STATE_IDLE,
        .counter = 0,
        .max_iterations = iterations,
        .error_threshold = threshold
    };
    
    run_state_machine(&ctx);
    
    switch (ctx.current_state) {
        case STATE_COMPLETED:
            printf("State machine completed successfully after %u iterations\n", ctx.counter);
            break;
        case STATE_ERROR:
            printf("State machine encountered error at iteration %u\n", ctx.counter);
            break;
        default:
            printf("Unexpected final state: %d\n", ctx.current_state);
            break;
    }
    
    return EXIT_SUCCESS;
}