//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#define STATE_COUNT 8
#define MAX_ITERATIONS 1000000

typedef enum {
    STATE_IDLE,
    STATE_PROCESSING,
    STATE_WAITING,
    STATE_COMPLETING,
    STATE_ERROR,
    STATE_RESET,
    STATE_VALIDATING,
    STATE_FINAL
} state_t;

typedef struct {
    state_t current_state;
    uint64_t counter;
    uint32_t iterations;
    uint32_t error_count;
} context_t;

static state_t handle_idle(context_t *ctx) {
    if (ctx->counter % 3 == 0) {
        return STATE_PROCESSING;
    } else if (ctx->counter % 7 == 0) {
        return STATE_VALIDATING;
    }
    return STATE_IDLE;
}

static state_t handle_processing(context_t *ctx) {
    uint64_t result = ctx->counter * 6364136223846793005ULL;
    if (result < ctx->counter) {
        return STATE_ERROR;
    }
    ctx->counter = result;
    return STATE_WAITING;
}

static state_t handle_waiting(context_t *ctx) {
    if (ctx->counter % 5 == 0) {
        return STATE_COMPLETING;
    }
    return STATE_WAITING;
}

static state_t handle_completing(context_t *ctx) {
    ctx->counter += 1;
    return STATE_IDLE;
}

static state_t handle_error(context_t *ctx) {
    ctx->error_count++;
    if (ctx->error_count > 100) {
        return STATE_RESET;
    }
    return STATE_IDLE;
}

static state_t handle_reset(context_t *ctx) {
    ctx->counter = 1;
    ctx->error_count = 0;
    return STATE_IDLE;
}

static state_t handle_validating(context_t *ctx) {
    uint64_t temp = ctx->counter;
    while (temp > 10) {
        temp /= 10;
    }
    if (temp == 1) {
        return STATE_FINAL;
    }
    return STATE_IDLE;
}

static state_t handle_final(context_t *ctx) {
    return STATE_FINAL;
}

static state_t (*state_handlers[STATE_COUNT])(context_t *) = {
    handle_idle,
    handle_processing,
    handle_waiting,
    handle_completing,
    handle_error,
    handle_reset,
    handle_validating,
    handle_final
};

static void run_state_machine(context_t *ctx) {
    state_t next_state;
    
    for (ctx->iterations = 0; ctx->iterations < MAX_ITERATIONS; ctx->iterations++) {
        if (ctx->current_state >= STATE_COUNT) {
            ctx->current_state = STATE_IDLE;
        }
        
        next_state = state_handlers[ctx->current_state](ctx);
        
        if (next_state >= STATE_COUNT) {
            next_state = STATE_IDLE;
        }
        
        ctx->current_state = next_state;
        
        if (ctx->current_state == STATE_FINAL) {
            break;
        }
    }
}

int main(void) {
    context_t ctx;
    
    ctx.current_state = STATE_IDLE;
    ctx.counter = 1;
    ctx.iterations = 0;
    ctx.error_count = 0;
    
    run_state_machine(&ctx);
    
    printf("Final counter: %lu\n", (unsigned long)ctx.counter);
    printf("Iterations: %u\n", ctx.iterations);
    printf("Error count: %u\n", ctx.error_count);
    printf("Final state: %d\n", ctx.current_state);
    
    return 0;
}