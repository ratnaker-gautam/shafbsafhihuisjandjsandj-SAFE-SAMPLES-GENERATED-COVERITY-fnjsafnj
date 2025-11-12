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
    
    uint32_t i;
    for (i = 0; i < 1000; i++) {
        ctx->counter++;
        if (ctx->counter >= ctx->max_iterations) break;
    }
    
    if (ctx->counter % ctx->error_threshold == 0 && ctx->counter > 0) {
        return STATE_ERROR;
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

static void run_state_machine(struct context *ctx) {
    if (ctx == NULL) return;
    
    while (1) {
        switch (ctx->current_state) {
            case STATE_IDLE:
                ctx->current_state = handle_idle(ctx);
                break;
            case STATE_PROCESSING:
                ctx->current_state = handle_processing(ctx);
                break;
            case STATE_COMPLETED:
                ctx->current_state = handle_completed(ctx);
                break;
            case STATE_ERROR:
                ctx->current_state = handle_error(ctx);
                break;
            default:
                ctx->current_state = STATE_ERROR;
                break;
        }
        
        if (ctx->current_state == STATE_COMPLETED || 
            ctx->current_state == STATE_ERROR) {
            break;
        }
    }
}

int main(void) {
    uint32_t iterations, threshold;
    char input_buffer[64];
    
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
    
    struct context ctx = {
        .current_state = STATE_IDLE,
        .counter = 0,
        .max_iterations = iterations,
        .error_threshold = threshold
    };
    
    run_state_machine(&ctx);
    
    if (ctx.current_state == STATE_COMPLETED) {
        printf("Processing completed successfully. Final count: %u\n", ctx.counter);
        return EXIT_SUCCESS;
    } else {
        printf("Processing failed with error. Final count: %u\n", ctx.counter);
        return EXIT_FAILURE;
    }
}