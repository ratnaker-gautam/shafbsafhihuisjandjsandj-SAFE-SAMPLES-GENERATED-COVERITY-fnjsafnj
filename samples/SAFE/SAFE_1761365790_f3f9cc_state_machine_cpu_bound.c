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
    if (ctx->counter >= ctx->max_iterations) {
        ctx->current_state = STATE_COMPLETED;
        return;
    }
    if (ctx->counter % ctx->error_threshold == 0) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    ctx->current_state = STATE_PROCESSING;
}

static void state_processing(struct context *ctx) {
    uint32_t i, sum = 0;
    for (i = 0; i < 1000; i++) {
        sum += i;
        if (sum > UINT32_MAX / 2) sum = 0;
    }
    ctx->counter++;
    ctx->current_state = STATE_IDLE;
}

static void state_completed(struct context *ctx) {
    printf("Processing completed. Total iterations: %u\n", ctx->counter);
}

static void state_error(struct context *ctx) {
    printf("Error condition at iteration %u\n", ctx->counter);
    ctx->current_state = STATE_IDLE;
}

static void run_state_machine(struct context *ctx) {
    while (ctx->current_state != STATE_COMPLETED) {
        switch (ctx->current_state) {
            case STATE_IDLE:
                state_idle(ctx);
                break;
            case STATE_PROCESSING:
                state_processing(ctx);
                break;
            case STATE_COMPLETED:
                state_completed(ctx);
                break;
            case STATE_ERROR:
                state_error(ctx);
                break;
            default:
                ctx->current_state = STATE_ERROR;
                break;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <iterations> <error_threshold>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *endptr;
    unsigned long iter_val = strtoul(argv[1], &endptr, 10);
    if (*endptr != '\0' || iter_val == 0 || iter_val > 1000000) {
        fprintf(stderr, "Invalid iterations value. Must be 1-1000000\n");
        return EXIT_FAILURE;
    }

    unsigned long thresh_val = strtoul(argv[2], &endptr, 10);
    if (*endptr != '\0' || thresh_val == 0 || thresh_val > iter_val) {
        fprintf(stderr, "Invalid threshold value. Must be 1-%lu\n", iter_val);
        return EXIT_FAILURE;
    }

    struct context ctx;
    ctx.current_state = STATE_IDLE;
    ctx.counter = 0;
    ctx.max_iterations = (uint32_t)iter_val;
    ctx.error_threshold = (uint32_t)thresh_val;

    run_state_machine(&ctx);

    return EXIT_SUCCESS;
}