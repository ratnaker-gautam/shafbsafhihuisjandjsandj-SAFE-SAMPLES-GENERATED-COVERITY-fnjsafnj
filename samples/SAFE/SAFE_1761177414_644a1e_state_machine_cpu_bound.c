//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define STATE_COUNT 8
#define MAX_ITERATIONS 1000000

typedef enum {
    STATE_START,
    STATE_PROCESSING,
    STATE_VALIDATING,
    STATE_COMPUTING,
    STATE_AGGREGATING,
    STATE_FINALIZING,
    STATE_ERROR,
    STATE_DONE
} state_t;

typedef struct {
    state_t current_state;
    uint64_t counter;
    uint32_t iterations;
    int error_code;
} context_t;

static state_t handle_start(context_t *ctx) {
    if (ctx->counter > UINT64_MAX - 1000) {
        return STATE_ERROR;
    }
    ctx->counter += 1;
    return STATE_PROCESSING;
}

static state_t handle_processing(context_t *ctx) {
    if (ctx->counter > UINT64_MAX - 10000) {
        return STATE_ERROR;
    }
    ctx->counter += ctx->iterations % 100;
    return STATE_VALIDATING;
}

static state_t handle_validating(context_t *ctx) {
    if (ctx->counter % 2 == 0) {
        return STATE_COMPUTING;
    } else {
        if (ctx->counter > UINT64_MAX - 5000) {
            return STATE_ERROR;
        }
        ctx->counter += 1000;
        return STATE_AGGREGATING;
    }
}

static state_t handle_computing(context_t *ctx) {
    uint64_t temp = ctx->counter;
    for (int i = 0; i < 100; i++) {
        if (temp > UINT64_MAX / 2) {
            return STATE_ERROR;
        }
        temp = temp * 3 + 1;
    }
    ctx->counter = temp;
    return STATE_AGGREGATING;
}

static state_t handle_aggregating(context_t *ctx) {
    if (ctx->counter > UINT64_MAX - ctx->iterations) {
        return STATE_ERROR;
    }
    ctx->counter += ctx->iterations;
    return STATE_FINALIZING;
}

static state_t handle_finalizing(context_t *ctx) {
    if (ctx->counter > 1000000) {
        ctx->counter = ctx->counter % 1000000;
    }
    return STATE_DONE;
}

static state_t handle_error(context_t *ctx) {
    ctx->error_code = 1;
    return STATE_DONE;
}

static state_t handle_done(context_t *ctx) {
    return STATE_DONE;
}

static state_t (*state_handlers[STATE_COUNT])(context_t *) = {
    handle_start,
    handle_processing,
    handle_validating,
    handle_computing,
    handle_aggregating,
    handle_finalizing,
    handle_error,
    handle_done
};

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <iterations>\n", argv[0]);
        return 1;
    }

    char *endptr;
    long input_iterations = strtol(argv[1], &endptr, 10);
    if (*endptr != '\0' || input_iterations <= 0 || input_iterations > MAX_ITERATIONS) {
        fprintf(stderr, "Error: iterations must be between 1 and %d\n", MAX_ITERATIONS);
        return 1;
    }

    context_t ctx;
    ctx.current_state = STATE_START;
    ctx.counter = 0;
    ctx.iterations = (uint32_t)input_iterations;
    ctx.error_code = 0;

    while (ctx.current_state != STATE_DONE) {
        if (ctx.current_state < 0 || ctx.current_state >= STATE_COUNT) {
            fprintf(stderr, "Invalid state encountered\n");
            return 1;
        }
        ctx.current_state = state_handlers[ctx.current_state](&ctx);
    }

    if (ctx.error_code != 0) {
        fprintf(stderr, "Computation failed with error\n");
        return 1;
    }

    printf("Final counter value: %lu\n", ctx.counter);
    return 0;
}