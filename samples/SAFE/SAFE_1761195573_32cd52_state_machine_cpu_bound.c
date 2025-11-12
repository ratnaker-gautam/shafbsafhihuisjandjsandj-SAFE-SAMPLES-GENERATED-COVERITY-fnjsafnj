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
    STATE_SUSPEND,
    STATE_TERMINATE
} state_t;

typedef struct {
    state_t current_state;
    uint64_t counter;
    uint32_t iterations;
    uint8_t error_code;
} context_t;

static state_t handle_idle(context_t *ctx) {
    if (ctx->counter % 3 == 0) {
        return STATE_PROCESSING;
    } else if (ctx->counter % 7 == 0) {
        return STATE_ERROR;
    }
    return STATE_IDLE;
}

static state_t handle_processing(context_t *ctx) {
    uint64_t result = ctx->counter * 3;
    if (result < ctx->counter) {
        return STATE_ERROR;
    }
    ctx->counter = result;
    
    if (ctx->counter % 5 == 0) {
        return STATE_WAITING;
    }
    return STATE_PROCESSING;
}

static state_t handle_waiting(context_t *ctx) {
    if (ctx->counter % 11 == 0) {
        return STATE_COMPLETING;
    }
    return STATE_WAITING;
}

static state_t handle_completing(context_t *ctx) {
    ctx->counter += 1;
    if (ctx->counter % 13 == 0) {
        return STATE_IDLE;
    }
    return STATE_COMPLETING;
}

static state_t handle_error(context_t *ctx) {
    ctx->error_code = (uint8_t)(ctx->counter % 256);
    if (ctx->counter % 17 == 0) {
        return STATE_RESET;
    }
    return STATE_ERROR;
}

static state_t handle_reset(context_t *ctx) {
    ctx->counter = 1;
    ctx->error_code = 0;
    return STATE_IDLE;
}

static state_t handle_suspend(context_t *ctx) {
    if (ctx->counter % 19 == 0) {
        return STATE_PROCESSING;
    }
    return STATE_SUSPEND;
}

static state_t handle_terminate(context_t *ctx) {
    return STATE_TERMINATE;
}

static state_t (*state_handlers[STATE_COUNT])(context_t *) = {
    handle_idle,
    handle_processing,
    handle_waiting,
    handle_completing,
    handle_error,
    handle_reset,
    handle_suspend,
    handle_terminate
};

static void run_state_machine(context_t *ctx) {
    state_t next_state;
    
    for (ctx->iterations = 0; ctx->iterations < MAX_ITERATIONS; ctx->iterations++) {
        if (ctx->current_state >= STATE_COUNT) {
            ctx->current_state = STATE_ERROR;
        }
        
        next_state = state_handlers[ctx->current_state](ctx);
        
        if (next_state >= STATE_COUNT) {
            ctx->current_state = STATE_ERROR;
            continue;
        }
        
        ctx->current_state = next_state;
        
        if (ctx->current_state == STATE_TERMINATE) {
            break;
        }
        
        if (ctx->counter >= UINT64_MAX - 1000) {
            ctx->current_state = STATE_RESET;
        }
    }
}

int main(void) {
    context_t ctx;
    
    ctx.current_state = STATE_IDLE;
    ctx.counter = 1;
    ctx.iterations = 0;
    ctx.error_code = 0;
    
    run_state_machine(&ctx);
    
    printf("Final state: %d\n", ctx.current_state);
    printf("Counter value: %lu\n", ctx.counter);
    printf("Iterations: %u\n", ctx.iterations);
    printf("Error code: %u\n", ctx.error_code);
    
    return 0;
}