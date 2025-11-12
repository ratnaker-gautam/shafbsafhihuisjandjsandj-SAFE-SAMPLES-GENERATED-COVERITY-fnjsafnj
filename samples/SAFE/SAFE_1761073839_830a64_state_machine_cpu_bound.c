//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

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
    uint64_t temp = ctx->counter;
    for (int i = 0; i < 100; i++) {
        temp = (temp * 1103515245 + 12345) % 2147483648;
    }
    ctx->counter = temp;
    return STATE_COMPUTING;
}

static state_t handle_validating(context_t *ctx) {
    if (ctx->counter == 0) {
        ctx->error_code = 1;
        return STATE_ERROR;
    }
    return STATE_AGGREGATING;
}

static state_t handle_computing(context_t *ctx) {
    uint64_t result = 0;
    for (uint64_t i = 1; i <= 1000; i++) {
        result += (ctx->counter * i) % 1000;
    }
    ctx->counter = result;
    return STATE_FINALIZING;
}

static state_t handle_aggregating(context_t *ctx) {
    uint64_t sum = 0;
    for (int i = 0; i < 100; i++) {
        sum += (ctx->counter + i) * i;
    }
    ctx->counter = sum;
    return STATE_COMPUTING;
}

static state_t handle_finalizing(context_t *ctx) {
    if (ctx->counter > 1000000) {
        return STATE_DONE;
    }
    return STATE_IDLE;
}

static state_t handle_error(context_t *ctx) {
    ctx->counter = 0;
    return STATE_IDLE;
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

int main(void) {
    context_t ctx;
    ctx.current_state = STATE_IDLE;
    ctx.counter = 1;
    ctx.iterations = 0;
    ctx.error_code = 0;

    printf("Starting state machine simulation...\n");

    while (ctx.current_state != STATE_DONE && ctx.iterations < MAX_ITERATIONS) {
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

        if (ctx.iterations % 100000 == 0) {
            printf("Progress: %u iterations, state: %d, counter: %lu\n", 
                   ctx.iterations, ctx.current_state, ctx.counter);
        }
    }

    if (ctx.iterations >= MAX_ITERATIONS) {
        printf("Reached maximum iterations\n");
    } else {
        printf("Completed in %u iterations. Final counter: %lu\n", 
               ctx.iterations, ctx.counter);
    }

    if (ctx.error_code != 0) {
        printf("Error occurred: %d\n", ctx.error_code);
        return 1;
    }

    return 0;
}