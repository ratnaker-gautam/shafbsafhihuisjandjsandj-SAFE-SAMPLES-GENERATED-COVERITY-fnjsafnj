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
    uint64_t counter;
    uint32_t max_iterations;
    uint32_t error_threshold;
};

static int validate_input(const char *input, uint32_t *value) {
    if (input == NULL) return 0;
    
    char *endptr;
    long long_val = strtol(input, &endptr, 10);
    
    if (endptr == input || *endptr != '\0') return 0;
    if (long_val < 0 || long_val > UINT32_MAX) return 0;
    
    *value = (uint32_t)long_val;
    return 1;
}

static void state_idle(struct context *ctx) {
    if (ctx->counter > 0) {
        ctx->current_state = STATE_PROCESSING;
    } else {
        ctx->current_state = STATE_COMPLETED;
    }
}

static void state_processing(struct context *ctx) {
    if (ctx->counter >= ctx->max_iterations) {
        ctx->current_state = STATE_COMPLETED;
        return;
    }
    
    if (ctx->counter >= ctx->error_threshold) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    uint64_t new_counter = ctx->counter + 1;
    if (new_counter <= ctx->counter) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    ctx->counter = new_counter;
    
    if (ctx->counter % 1000000 == 0) {
        printf("Progress: %lu/%u\n", ctx->counter, ctx->max_iterations);
    }
    
    if (ctx->counter >= ctx->max_iterations) {
        ctx->current_state = STATE_COMPLETED;
    }
}

static void state_completed(struct context *ctx) {
    printf("Processing completed. Final count: %lu\n", ctx->counter);
}

static void state_error(struct context *ctx) {
    printf("Error state reached at count: %lu\n", ctx->counter);
}

static void run_state_machine(struct context *ctx) {
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
    
    if (ctx->current_state == STATE_COMPLETED) {
        state_completed(ctx);
    } else if (ctx->current_state == STATE_ERROR) {
        state_error(ctx);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <max_iterations> <error_threshold>\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    uint32_t max_iterations;
    uint32_t error_threshold;
    
    if (!validate_input(argv[1], &max_iterations)) {
        fprintf(stderr, "Invalid max_iterations value\n");
        return EXIT_FAILURE;
    }
    
    if (!validate_input(argv[2], &error_threshold)) {
        fprintf(stderr, "Invalid error_threshold value\n");
        return EXIT_FAILURE;
    }
    
    if (max_iterations == 0) {
        fprintf(stderr, "max_iterations must be greater than 0\n");
        return EXIT_FAILURE;
    }
    
    if (error_threshold > max_iterations) {
        fprintf(stderr, "error_threshold cannot exceed max_iterations\n");
        return EXIT_FAILURE;
    }
    
    struct context ctx = {
        .current_state = STATE_IDLE,
        .counter = 0,
        .max_iterations = max_iterations,
        .error_threshold = error_threshold
    };
    
    run_state_machine(&ctx);
    
    return ctx.current_state == STATE_COMPLETED ? EXIT_SUCCESS : EXIT_FAILURE;
}