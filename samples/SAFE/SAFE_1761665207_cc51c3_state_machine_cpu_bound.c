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

static int validate_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > 10) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] < '0' || input[i] > '9') return 0;
    }
    return 1;
}

static uint32_t parse_uint32(const char *input) {
    unsigned long long val = 0;
    for (size_t i = 0; input[i] != '\0'; i++) {
        if (val > UINT32_MAX / 10) return UINT32_MAX;
        val = val * 10 + (input[i] - '0');
        if (val > UINT32_MAX) return UINT32_MAX;
    }
    return (uint32_t)val;
}

static void state_idle(struct context *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = STATE_PROCESSING;
    ctx->counter = 0;
}

static void state_processing(struct context *ctx) {
    if (ctx == NULL) return;
    if (ctx->counter >= ctx->max_iterations) {
        ctx->current_state = STATE_COMPLETED;
        return;
    }
    uint32_t result = 0;
    for (uint32_t i = 0; i < ctx->counter; i++) {
        result += i * i;
        if (result > UINT32_MAX / 2) {
            result = 0;
        }
    }
    ctx->counter++;
    if (ctx->counter % ctx->error_threshold == 0) {
        ctx->current_state = STATE_ERROR;
    }
}

static void state_completed(struct context *ctx) {
    if (ctx == NULL) return;
}

static void state_error(struct context *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = STATE_IDLE;
}

static void run_state_machine(struct context *ctx) {
    if (ctx == NULL) return;
    uint32_t cycle_count = 0;
    while (ctx->current_state != STATE_COMPLETED && cycle_count < 1000000) {
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
        cycle_count++;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <iterations> <error_threshold>\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (!validate_input(argv[1]) || !validate_input(argv[2])) {
        fprintf(stderr, "Error: Invalid input parameters\n");
        return EXIT_FAILURE;
    }

    uint32_t iterations = parse_uint32(argv[1]);
    uint32_t error_threshold = parse_uint32(argv[2]);

    if (iterations == 0 || iterations > 1000000) {
        fprintf(stderr, "Error: Iterations must be between 1 and 1000000\n");
        return EXIT_FAILURE;
    }

    if (error_threshold == 0 || error_threshold > 1000) {
        fprintf(stderr, "Error: Threshold must be between 1 and 1000\n");
        return EXIT_FAILURE;
    }

    struct context ctx;
    ctx.current_state = STATE_IDLE;
    ctx.max_iterations = iterations;
    ctx.error_threshold = error_threshold;

    run_state_machine(&ctx);

    if (ctx.current_state == STATE_COMPLETED) {
        printf("State machine completed successfully after %u iterations\n", ctx.counter);
    } else {
        printf("State machine terminated in state %d after %u iterations\n", ctx.current_state, ctx.counter);
    }

    return EXIT_SUCCESS;
}