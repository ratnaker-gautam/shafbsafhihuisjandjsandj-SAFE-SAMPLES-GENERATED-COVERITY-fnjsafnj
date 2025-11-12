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
    if (ctx->counter >= ctx->max_iterations) {
        ctx->current_state = STATE_COMPLETED;
    } else if (ctx->counter % ctx->error_threshold == 0) {
        ctx->current_state = STATE_ERROR;
    } else {
        ctx->current_state = STATE_PROCESSING;
    }
}

static void state_processing(struct context *ctx) {
    uint32_t old_counter = ctx->counter;
    if (ctx->counter < UINT32_MAX - 1000) {
        for (int i = 0; i < 1000; i++) {
            ctx->counter++;
        }
    } else {
        ctx->counter = UINT32_MAX;
    }
    if (ctx->counter == old_counter) {
        ctx->current_state = STATE_ERROR;
    } else if (ctx->counter >= ctx->max_iterations) {
        ctx->current_state = STATE_COMPLETED;
    } else {
        ctx->current_state = STATE_IDLE;
    }
}

static void state_completed(struct context *ctx) {
    printf("Processing completed. Final counter: %u\n", ctx->counter);
}

static void state_error(struct context *ctx) {
    printf("Error state reached at counter: %u\n", ctx->counter);
    ctx->current_state = STATE_COMPLETED;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <max_iterations> <error_threshold>\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (!validate_input(argv[1]) || !validate_input(argv[2])) {
        fprintf(stderr, "Invalid input. Please provide positive integers.\n");
        return EXIT_FAILURE;
    }

    uint32_t max_iterations = parse_uint32(argv[1]);
    uint32_t error_threshold = parse_uint32(argv[2]);

    if (max_iterations == 0 || error_threshold == 0) {
        fprintf(stderr, "Values must be greater than zero.\n");
        return EXIT_FAILURE;
    }

    if (error_threshold > max_iterations) {
        fprintf(stderr, "Error threshold cannot exceed max iterations.\n");
        return EXIT_FAILURE;
    }

    struct context ctx = {
        .current_state = STATE_IDLE,
        .counter = 0,
        .max_iterations = max_iterations,
        .error_threshold = error_threshold
    };

    while (ctx.current_state != STATE_COMPLETED) {
        switch (ctx.current_state) {
            case STATE_IDLE:
                state_idle(&ctx);
                break;
            case STATE_PROCESSING:
                state_processing(&ctx);
                break;
            case STATE_COMPLETED:
                state_completed(&ctx);
                break;
            case STATE_ERROR:
                state_error(&ctx);
                break;
            default:
                fprintf(stderr, "Invalid state encountered.\n");
                return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}