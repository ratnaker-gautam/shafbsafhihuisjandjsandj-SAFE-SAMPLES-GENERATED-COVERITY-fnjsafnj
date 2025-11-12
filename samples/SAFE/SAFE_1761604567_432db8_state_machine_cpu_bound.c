//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define STATE_COUNT 8
#define MAX_ITERATIONS 1000000

typedef enum {
    STATE_IDLE,
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

static state_t handle_idle(context_t *ctx) {
    if (ctx->counter % 2 == 0) {
        return STATE_PROCESSING;
    }
    return STATE_VALIDATING;
}

static state_t handle_processing(context_t *ctx) {
    uint64_t temp = ctx->counter * 3;
    if (temp < ctx->counter) return STATE_ERROR;
    ctx->counter = temp + 1;
    return STATE_VALIDATING;
}

static state_t handle_validating(context_t *ctx) {
    if (ctx->counter % 7 == 0) {
        return STATE_COMPUTING;
    }
    if (ctx->counter % 13 == 0) {
        return STATE_AGGREGATING;
    }
    return STATE_PROCESSING;
}

static state_t handle_computing(context_t *ctx) {
    for (int i = 0; i < 100; i++) {
        uint64_t square = ctx->counter * ctx->counter;
        if (square / ctx->counter != ctx->counter) {
            return STATE_ERROR;
        }
        ctx->counter = (ctx->counter + square) / 2;
    }
    return STATE_AGGREGATING;
}

static state_t handle_aggregating(context_t *ctx) {
    uint64_t sum = 0;
    for (int i = 0; i < 50; i++) {
        sum += ctx->counter + i;
        if (sum < ctx->counter) return STATE_ERROR;
    }
    ctx->counter = sum;
    return STATE_FINALIZING;
}

static state_t handle_finalizing(context_t *ctx) {
    if (ctx->counter > 1000000000ULL) {
        return STATE_DONE;
    }
    return STATE_PROCESSING;
}

static state_t handle_error(context_t *ctx) {
    ctx->error_code = (int)(ctx->counter % 256);
    return STATE_DONE;
}

static state_t handle_done(context_t *ctx) {
    return STATE_DONE;
}

static state_t (*state_handlers[STATE_COUNT])(context_t *) = {
    handle_idle,
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
        fprintf(stderr, "Usage: %s <initial_value>\n", argv[0]);
        return 1;
    }

    char *endptr;
    unsigned long initial = strtoul(argv[1], &endptr, 10);
    if (*endptr != '\0' || initial == 0 || initial > 1000000) {
        fprintf(stderr, "Invalid initial value. Must be 1-1000000\n");
        return 1;
    }

    context_t ctx;
    ctx.current_state = STATE_IDLE;
    ctx.counter = initial;
    ctx.iterations = 0;
    ctx.error_code = 0;

    while (ctx.current_state != STATE_DONE && ctx.current_state != STATE_ERROR) {
        if (ctx.iterations >= MAX_ITERATIONS) {
            fprintf(stderr, "Maximum iterations exceeded\n");
            return 1;
        }

        if (ctx.current_state >= STATE_COUNT) {
            fprintf(stderr, "Invalid state encountered\n");
            return 1;
        }

        state_t next_state = state_handlers[ctx.current_state](&ctx);
        if (next_state >= STATE_COUNT) {
            fprintf(stderr, "Invalid next state\n");
            return 1;
        }

        ctx.current_state = next_state;
        ctx.iterations++;

        if (ctx.counter == 0) {
            fprintf(stderr, "Counter reached zero\n");
            return 1;
        }
    }

    if (ctx.current_state == STATE_ERROR) {
        printf("Error state reached with code: %d\n", ctx.error_code);
        printf("Final counter: %lu\n", (unsigned long)ctx.counter);
        printf("Iterations: %u\n", ctx.iterations);
        return 1;
    }

    printf("Successfully completed state machine\n");
    printf("Final counter: %lu\n", (unsigned long)ctx.counter);
    printf("Iterations: %u\n", ctx.iterations);
    return 0;
}