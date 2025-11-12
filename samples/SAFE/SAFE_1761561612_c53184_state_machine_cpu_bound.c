//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
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

static int validate_input(const char *input, uint32_t *value) {
    if (input == NULL) return 0;
    char *endptr;
    long long_val = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\0') return 0;
    if (long_val < 0 || long_val > UINT32_MAX) return 0;
    *value = (uint32_t)long_val;
    return 1;
}

static enum state handle_idle(struct context *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    if (ctx->counter >= ctx->max_iterations) return STATE_COMPLETE;
    return STATE_PROCESSING;
}

static enum state handle_processing(struct context *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    if (ctx->counter >= ctx->max_iterations) return STATE_COMPLETE;
    ctx->counter++;
    if (ctx->counter % ctx->error_threshold == 0) return STATE_ERROR;
    if (ctx->counter >= ctx->max_iterations) return STATE_COMPLETE;
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
    
    while (ctx->current_state != STATE_COMPLETE && ctx->current_state != STATE_ERROR) {
        switch (ctx->current_state) {
            case STATE_IDLE:
                ctx->current_state = handle_idle(ctx);
                break;
            case STATE_PROCESSING:
                ctx->current_state = handle_processing(ctx);
                break;
            case STATE_COMPLETE:
                ctx->current_state = handle_complete(ctx);
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

    struct context ctx;
    ctx.counter = 0;
    ctx.current_state = STATE_IDLE;

    if (!validate_input(argv[1], &ctx.max_iterations)) {
        fprintf(stderr, "Invalid max_iterations value\n");
        return EXIT_FAILURE;
    }

    if (!validate_input(argv[2], &ctx.error_threshold)) {
        fprintf(stderr, "Invalid error_threshold value\n");
        return EXIT_FAILURE;
    }

    if (ctx.max_iterations == 0) {
        fprintf(stderr, "max_iterations must be greater than 0\n");
        return EXIT_FAILURE;
    }

    if (ctx.error_threshold == 0) {
        fprintf(stderr, "error_threshold must be greater than 0\n");
        return EXIT_FAILURE;
    }

    run_state_machine(&ctx);

    if (ctx.current_state == STATE_COMPLETE) {
        printf("State machine completed successfully after %u iterations\n", ctx.counter);
        return EXIT_SUCCESS;
    } else {
        printf("State machine encountered error at iteration %u\n", ctx.counter);
        return EXIT_FAILURE;
    }
}