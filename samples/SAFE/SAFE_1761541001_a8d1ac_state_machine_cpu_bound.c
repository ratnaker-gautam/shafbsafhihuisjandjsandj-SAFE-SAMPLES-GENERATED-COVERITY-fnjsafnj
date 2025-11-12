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
    if (ctx == NULL) return;
    printf("State: IDLE\n");
    if (ctx->counter < ctx->max_iterations) {
        ctx->current_state = STATE_PROCESSING;
    } else {
        ctx->current_state = STATE_COMPLETED;
    }
}

static void state_processing(struct context *ctx) {
    if (ctx == NULL) return;
    printf("State: PROCESSING - Iteration %u\n", ctx->counter);
    ctx->counter++;
    if (ctx->counter > ctx->max_iterations) {
        ctx->current_state = STATE_COMPLETED;
    } else if (ctx->counter % ctx->error_threshold == 0) {
        ctx->current_state = STATE_ERROR;
    } else {
        ctx->current_state = STATE_PROCESSING;
    }
}

static void state_completed(struct context *ctx) {
    if (ctx == NULL) return;
    printf("State: COMPLETED - Total iterations: %u\n", ctx->counter);
}

static void state_error(struct context *ctx) {
    if (ctx == NULL) return;
    printf("State: ERROR at iteration %u\n", ctx->counter);
    ctx->current_state = STATE_IDLE;
}

static void run_state_machine(struct context *ctx) {
    if (ctx == NULL) return;
    uint32_t step_count = 0;
    const uint32_t max_steps = 1000;
    
    while (ctx->current_state != STATE_COMPLETED && step_count < max_steps) {
        switch (ctx->current_state) {
            case STATE_IDLE:
                state_idle(ctx);
                break;
            case STATE_PROCESSING:
                state_processing(ctx);
                break;
            case STATE_COMPLETED:
                break;
            case STATE_ERROR:
                state_error(ctx);
                break;
            default:
                ctx->current_state = STATE_ERROR;
                break;
        }
        step_count++;
    }
    
    if (step_count >= max_steps) {
        printf("State machine terminated after maximum steps\n");
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
    
    if (error_threshold == 0) {
        fprintf(stderr, "error_threshold must be greater than 0\n");
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
    
    return EXIT_SUCCESS;
}