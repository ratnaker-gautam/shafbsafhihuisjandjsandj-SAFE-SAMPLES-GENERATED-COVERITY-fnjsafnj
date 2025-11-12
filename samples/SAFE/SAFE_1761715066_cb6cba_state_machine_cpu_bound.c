//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

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
    if (ctx->counter >= UINT64_MAX - 1000) {
        return STATE_ERROR;
    }
    ctx->counter += 1;
    return STATE_PROCESSING;
}

static state_t handle_processing(context_t *ctx) {
    if (ctx->counter % 2 == 0) {
        ctx->counter *= 3;
    } else {
        ctx->counter += 7;
    }
    if (ctx->counter > UINT64_MAX / 3) {
        return STATE_ERROR;
    }
    return STATE_VALIDATING;
}

static state_t handle_validating(context_t *ctx) {
    if (ctx->counter == 0 || ctx->counter > UINT64_MAX - 1000000) {
        return STATE_ERROR;
    }
    uint64_t temp = ctx->counter;
    while (temp > 1) {
        if (temp % 2 == 0) {
            temp /= 2;
        } else {
            temp = temp * 3 + 1;
        }
        if (temp > UINT64_MAX / 3) {
            return STATE_ERROR;
        }
    }
    return STATE_COMPUTING;
}

static state_t handle_computing(context_t *ctx) {
    uint64_t sum = 0;
    for (uint32_t i = 1; i <= 1000; i++) {
        if (sum > UINT64_MAX - i) {
            return STATE_ERROR;
        }
        sum += i;
    }
    ctx->counter += sum;
    if (ctx->counter < sum) {
        return STATE_ERROR;
    }
    return STATE_AGGREGATING;
}

static state_t handle_aggregating(context_t *ctx) {
    uint64_t product = 1;
    for (uint32_t i = 1; i <= 50; i++) {
        if (product > UINT64_MAX / i) {
            return STATE_ERROR;
        }
        product *= i;
    }
    ctx->counter += product;
    if (ctx->counter < product) {
        return STATE_ERROR;
    }
    return STATE_FINALIZING;
}

static state_t handle_finalizing(context_t *ctx) {
    if (ctx->counter % 1000000 == 0) {
        ctx->counter /= 1000;
    } else {
        ctx->counter %= 1000000;
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
    handle_idle,
    handle_processing,
    handle_validating,
    handle_computing,
    handle_aggregating,
    handle_finalizing,
    handle_error,
    handle_done
};

static void run_state_machine(context_t *ctx) {
    while (ctx->current_state != STATE_DONE && ctx->current_state != STATE_ERROR) {
        if (ctx->iterations >= MAX_ITERATIONS) {
            ctx->current_state = STATE_ERROR;
            break;
        }
        
        state_t next_state = state_handlers[ctx->current_state](ctx);
        if (next_state >= STATE_COUNT || next_state < 0) {
            ctx->current_state = STATE_ERROR;
            break;
        }
        
        ctx->current_state = next_state;
        ctx->iterations++;
        
        if (ctx->iterations % 10000 == 0) {
            printf("Iteration: %u, State: %d, Counter: %lu\n", 
                   ctx->iterations, ctx->current_state, ctx->counter);
        }
    }
}

int main(void) {
    context_t ctx;
    ctx.current_state = STATE_IDLE;
    ctx.counter = 1;
    ctx.iterations = 0;
    ctx.error_code = 0;
    
    printf("Starting state machine simulation\n");
    run_state_machine(&ctx);
    
    if (ctx.error_code != 0) {
        printf("State machine terminated with error\n");
        return EXIT_FAILURE;
    }
    
    printf("State machine completed successfully\n");
    printf("Final counter value: %lu\n", ctx.counter);
    printf("Total iterations: %u\n", ctx.iterations);
    
    return EXIT_SUCCESS;
}