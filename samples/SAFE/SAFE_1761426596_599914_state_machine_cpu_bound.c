//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
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

static int validate_input(const char *input, uint32_t *value) {
    if (input == NULL || value == NULL) return 0;
    
    char *endptr;
    long long_val = strtol(input, &endptr, 10);
    
    if (endptr == input || *endptr != '\0') return 0;
    if (long_val < 0 || long_val > UINT32_MAX) return 0;
    
    *value = (uint32_t)long_val;
    return 1;
}

static enum state handle_idle(struct context *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    if (ctx->counter >= ctx->max_iterations) {
        return STATE_COMPLETED;
    }
    
    if (ctx->counter % ctx->error_threshold == 0 && ctx->counter > 0) {
        return STATE_ERROR;
    }
    
    return STATE_PROCESSING;
}

static enum state handle_processing(struct context *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    for (volatile int i = 0; i < 1000; i++) {
        ctx->counter = ctx->counter + 1;
    }
    
    if (ctx->counter >= ctx->max_iterations) {
        return STATE_COMPLETED;
    }
    
    return STATE_IDLE;
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
    
    while (ctx->current_state != STATE_COMPLETED && 
           ctx->current_state != STATE_ERROR) {
        
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
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <max_iterations> <error_threshold>\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    uint32_t max_iterations, error_threshold;
    
    if (!validate_input(argv[1], &max_iterations)) {
        fprintf(stderr, "Invalid max_iterations value\n");
        return EXIT_FAILURE;
    }
    
    if (!validate_input(argv[2], &error_threshold)) {
        fprintf(stderr, "Invalid error_threshold value\n");
        return EXIT_FAILURE;
    }
    
    if (error_threshold == 0) {
        fprintf(stderr, "Error threshold cannot be zero\n");
        return EXIT_FAILURE;
    }
    
    if (max_iterations > 1000000) {
        fprintf(stderr, "Max iterations too large\n");
        return EXIT_FAILURE;
    }
    
    struct context ctx = {
        .current_state = STATE_IDLE,
        .counter = 0,
        .max_iterations = max_iterations,
        .error_threshold = error_threshold
    };
    
    run_state_machine(&ctx);
    
    if (ctx.current_state == STATE_COMPLETED) {
        printf("State machine completed successfully. Final counter: %u\n", ctx.counter);
        return EXIT_SUCCESS;
    } else {
        printf("State machine encountered an error. Final counter: %u\n", ctx.counter);
        return EXIT_FAILURE;
    }
}