//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define STATE_COUNT 8
#define MAX_ITERATIONS 1000000

typedef enum {
    STATE_INIT,
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
    uint32_t data[STATE_COUNT];
    int iteration;
} context_t;

static state_t handle_init(context_t *ctx) {
    if (ctx->counter > UINT64_MAX - 1000) return STATE_ERROR;
    ctx->counter += 1;
    memset(ctx->data, 0, sizeof(ctx->data));
    return STATE_PROCESSING;
}

static state_t handle_processing(context_t *ctx) {
    if (ctx->counter > UINT64_MAX - ctx->iteration) return STATE_ERROR;
    ctx->counter += ctx->iteration;
    for (int i = 0; i < STATE_COUNT; i++) {
        if (ctx->data[i] > UINT32_MAX - (i + 1)) return STATE_ERROR;
        ctx->data[i] += (i + 1);
    }
    return STATE_VALIDATING;
}

static state_t handle_validating(context_t *ctx) {
    uint32_t sum = 0;
    for (int i = 0; i < STATE_COUNT; i++) {
        if (sum > UINT32_MAX - ctx->data[i]) return STATE_ERROR;
        sum += ctx->data[i];
    }
    if (sum % 2 == 0) {
        return STATE_COMPUTING;
    }
    return STATE_ERROR;
}

static state_t handle_computing(context_t *ctx) {
    for (int i = 0; i < STATE_COUNT; i++) {
        if (ctx->data[i] > UINT32_MAX / 2) return STATE_ERROR;
        ctx->data[i] *= 2;
    }
    return STATE_AGGREGATING;
}

static state_t handle_aggregating(context_t *ctx) {
    uint64_t total = 0;
    for (int i = 0; i < STATE_COUNT; i++) {
        if (total > UINT64_MAX - ctx->data[i]) return STATE_ERROR;
        total += ctx->data[i];
    }
    ctx->counter = total;
    return STATE_FINALIZING;
}

static state_t handle_finalizing(context_t *ctx) {
    if (ctx->counter > 1000000) {
        return STATE_DONE;
    }
    return STATE_PROCESSING;
}

static state_t handle_error(context_t *ctx) {
    ctx->counter = 0;
    return STATE_DONE;
}

static state_t handle_done(context_t *ctx) {
    return STATE_DONE;
}

static state_t (*state_handlers[])(context_t*) = {
    handle_init,
    handle_processing,
    handle_validating,
    handle_computing,
    handle_aggregating,
    handle_finalizing,
    handle_error,
    handle_done
};

static void run_state_machine(context_t *ctx) {
    while (ctx->current_state != STATE_DONE && ctx->iteration < MAX_ITERATIONS) {
        if (ctx->current_state < 0 || ctx->current_state >= STATE_COUNT) {
            ctx->current_state = STATE_ERROR;
        }
        state_t next_state = state_handlers[ctx->current_state](ctx);
        ctx->current_state = next_state;
        ctx->iteration++;
    }
}

int main(void) {
    context_t ctx;
    ctx.current_state = STATE_INIT;
    ctx.counter = 0;
    ctx.iteration = 0;
    
    run_state_machine(&ctx);
    
    if (ctx.current_state == STATE_DONE) {
        printf("State machine completed successfully after %d iterations\n", ctx.iteration);
        printf("Final counter value: %lu\n", (unsigned long)ctx.counter);
    } else {
        printf("State machine terminated with error after %d iterations\n", ctx.iteration);
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}