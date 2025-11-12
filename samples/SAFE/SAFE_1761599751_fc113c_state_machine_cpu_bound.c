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

static state_t handle_idle(context_t *ctx);
static state_t handle_processing(context_t *ctx);
static state_t handle_waiting(context_t *ctx);
static state_t handle_completing(context_t *ctx);
static state_t handle_error(context_t *ctx);
static state_t handle_reset(context_t *ctx);
static state_t handle_suspend(context_t *ctx);
static state_t handle_terminate(context_t *ctx);

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

static state_t handle_idle(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    ctx->counter += 1;
    if (ctx->counter % 3 == 0) {
        return STATE_PROCESSING;
    }
    if (ctx->counter % 7 == 0) {
        return STATE_ERROR;
    }
    return STATE_IDLE;
}

static state_t handle_processing(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    for (int i = 0; i < 100; i++) {
        ctx->counter += i;
    }
    
    if (ctx->counter % 5 == 0) {
        return STATE_WAITING;
    }
    if (ctx->counter % 11 == 0) {
        return STATE_COMPLETING;
    }
    return STATE_PROCESSING;
}

static state_t handle_waiting(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    ctx->counter += 2;
    if (ctx->counter % 13 == 0) {
        return STATE_PROCESSING;
    }
    if (ctx->counter % 17 == 0) {
        return STATE_SUSPEND;
    }
    return STATE_WAITING;
}

static state_t handle_completing(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    ctx->counter *= 2;
    if (ctx->counter > 1000000) {
        return STATE_TERMINATE;
    }
    return STATE_RESET;
}

static state_t handle_error(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    ctx->error_code = (uint8_t)(ctx->counter % 256);
    if (ctx->counter % 19 == 0) {
        return STATE_RESET;
    }
    return STATE_ERROR;
}

static state_t handle_reset(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    ctx->counter = 0;
    ctx->error_code = 0;
    return STATE_IDLE;
}

static state_t handle_suspend(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    ctx->counter += 1;
    if (ctx->counter % 23 == 0) {
        return STATE_PROCESSING;
    }
    return STATE_SUSPEND;
}

static state_t handle_terminate(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    return STATE_TERMINATE;
}

int main(void) {
    context_t ctx;
    ctx.current_state = STATE_IDLE;
    ctx.counter = 0;
    ctx.iterations = 0;
    ctx.error_code = 0;
    
    printf("Starting state machine simulation\n");
    
    while (ctx.current_state != STATE_TERMINATE) {
        if (ctx.iterations >= MAX_ITERATIONS) {
            printf("Maximum iterations reached\n");
            break;
        }
        
        if (ctx.current_state >= STATE_COUNT) {
            printf("Invalid state detected\n");
            break;
        }
        
        state_t next_state = state_handlers[ctx.current_state](&ctx);
        ctx.current_state = next_state;
        ctx.iterations++;
        
        if (ctx.iterations % 100000 == 0) {
            printf("Iteration: %u, State: %d, Counter: %lu\n", 
                   ctx.iterations, ctx.current_state, ctx.counter);
        }
    }
    
    printf("Final state: %d, Total iterations: %u, Final counter: %lu\n", 
           ctx.current_state, ctx.iterations, ctx.counter);
    
    return 0;
}