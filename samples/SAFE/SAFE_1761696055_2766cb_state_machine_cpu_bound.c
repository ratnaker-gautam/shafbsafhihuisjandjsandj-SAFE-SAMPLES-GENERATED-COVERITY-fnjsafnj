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
    printf("State: IDLE\n");
    ctx->counter = 0;
    ctx->current_state = STATE_PROCESSING;
}

static void state_processing(struct context *ctx) {
    if (ctx == NULL) return;
    printf("State: PROCESSING (%u/%u)\n", ctx->counter + 1, ctx->max_iterations);
    
    for (volatile uint32_t i = 0; i < 1000; i++) {}
    
    ctx->counter++;
    
    if (ctx->counter >= ctx->max_iterations) {
        ctx->current_state = STATE_COMPLETED;
    } else if (ctx->counter >= ctx->error_threshold) {
        ctx->current_state = STATE_ERROR;
    }
}

static void state_completed(struct context *ctx) {
    if (ctx == NULL) return;
    printf("State: COMPLETED\n");
    printf("Processed %u iterations successfully\n", ctx->counter);
}

static void state_error(struct context *ctx) {
    if (ctx == NULL) return;
    printf("State: ERROR\n");
    printf("Stopped at iteration %u (threshold: %u)\n", ctx->counter, ctx->error_threshold);
}

int main(void) {
    struct context ctx;
    char input_buffer[64];
    uint32_t iterations, threshold;
    
    printf("Enter maximum iterations (1-1000000): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return EXIT_FAILURE;
    }
    
    if (sscanf(input_buffer, "%u", &iterations) != 1) {
        fprintf(stderr, "Invalid iteration count\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter error threshold (1-%u): ", iterations);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return EXIT_FAILURE;
    }
    
    if (sscanf(input_buffer, "%u", &threshold) != 1) {
        fprintf(stderr, "Invalid threshold\n");
        return EXIT_FAILURE;
    }
    
    if (!validate_input(iterations, threshold)) {
        fprintf(stderr, "Invalid parameters\n");
        return EXIT_FAILURE;
    }
    
    ctx.current_state = STATE_IDLE;
    ctx.max_iterations = iterations;
    ctx.error_threshold = threshold;
    
    while (ctx.current_state != STATE_COMPLETED && ctx.current_state != STATE_ERROR) {
        switch (ctx.current_state) {
            case STATE_IDLE:
                state_idle(&ctx);
                break;
            case STATE_PROCESSING:
                state_processing(&ctx);
                break;
            case STATE_COMPLETED:
            case STATE_ERROR:
                break;
            default:
                ctx.current_state = STATE_ERROR;
                break;
        }
    }
    
    if (ctx.current_state == STATE_COMPLETED) {
        state_completed(&ctx);
    } else if (ctx.current_state == STATE_ERROR) {
        state_error(&ctx);
    }
    
    return EXIT_SUCCESS;
}