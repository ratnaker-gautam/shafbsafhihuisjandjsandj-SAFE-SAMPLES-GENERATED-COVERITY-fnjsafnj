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
        ctx->error_code = 1;
        return STATE_ERROR;
    }
    ctx->counter += 1000;
    return STATE_PROCESSING;
}

static state_t handle_processing(context_t *ctx) {
    if (ctx->counter > UINT64_MAX / 2) {
        ctx->error_code = 2;
        return STATE_ERROR;
    }
    ctx->counter *= 2;
    return STATE_VALIDATING;
}

static state_t handle_validating(context_t *ctx) {
    uint64_t check = ctx->counter;
    for (int i = 0; i < 100; i++) {
        if (check > UINT64_MAX - i) {
            ctx->error_code = 3;
            return STATE_ERROR;
        }
        check += i;
    }
    return STATE_COMPUTING;
}

static state_t handle_computing(context_t *ctx) {
    uint64_t result = 0;
    for (int i = 1; i <= 1000; i++) {
        if (result > UINT64_MAX - (ctx->counter * i)) {
            ctx->error_code = 4;
            return STATE_ERROR;
        }
        result += ctx->counter * i;
    }
    ctx->counter = result;
    return STATE_AGGREGATING;
}

static state_t handle_aggregating(context_t *ctx) {
    uint64_t sum = 0;
    for (int i = 0; i < 100; i++) {
        if (sum > UINT64_MAX - (ctx->counter + i)) {
            ctx->error_code = 5;
            return STATE_ERROR;
        }
        sum += ctx->counter + i;
    }
    ctx->counter = sum;
    return STATE_FINALIZING;
}

static state_t handle_finalizing(context_t *ctx) {
    if (ctx->counter < 1000000) {
        ctx->counter = 1000000;
    }
    return STATE_DONE;
}

static state_t handle_error(context_t *ctx) {
    ctx->counter = 0;
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

static void run_state_machine(context_t *ctx) {
    while (ctx->current_state != STATE_DONE && ctx->iterations < MAX_ITERATIONS) {
        if (ctx->current_state < 0 || ctx->current_state >= STATE_COUNT) {
            ctx->error_code = 6;
            ctx->current_state = STATE_ERROR;
            continue;
        }
        
        state_t next_state = state_handlers[ctx->current_state](ctx);
        ctx->current_state = next_state;
        ctx->iterations++;
        
        if (ctx->iterations >= MAX_ITERATIONS) {
            ctx->error_code = 7;
            ctx->current_state = STATE_ERROR;
        }
    }
}

int main(void) {
    context_t ctx;
    ctx.current_state = STATE_START;
    ctx.counter = 1;
    ctx.iterations = 0;
    ctx.error_code = 0;
    
    run_state_machine(&ctx);
    
    if (ctx.error_code != 0) {
        printf("Error: %d\n", ctx.error_code);
        return 1;
    }
    
    printf("Final counter: %lu\n", ctx.counter);
    printf("Iterations: %u\n", ctx.iterations);
    
    return 0;
}