//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define STATE_COUNT 5
#define MAX_ITERATIONS 1000000

typedef enum {
    STATE_IDLE,
    STATE_PROCESSING,
    STATE_WAITING,
    STATE_COMPLETING,
    STATE_ERROR
} state_t;

typedef struct {
    state_t current_state;
    uint64_t counter;
    uint32_t iterations;
    int error_code;
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
    if (ctx->counter > UINT64_MAX / 2) {
        return STATE_ERROR;
    }
    
    uint64_t old_counter = ctx->counter;
    ctx->counter = ctx->counter * 3 + 1;
    
    if (ctx->counter < old_counter) {
        return STATE_ERROR;
    }
    
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
    if (ctx->counter % 13 == 0) {
        return STATE_IDLE;
    }
    return STATE_COMPLETING;
}

static state_t handle_error(context_t *ctx) {
    ctx->error_code = (int)(ctx->counter % 256);
    return STATE_ERROR;
}

static state_t (*state_handlers[STATE_COUNT])(context_t *) = {
    handle_idle,
    handle_processing,
    handle_waiting,
    handle_completing,
    handle_error
};

static void run_state_machine(context_t *ctx) {
    int state_changes = 0;
    
    for (ctx->iterations = 0; ctx->iterations < MAX_ITERATIONS; ctx->iterations++) {
        state_t old_state = ctx->current_state;
        state_t new_state = state_handlers[ctx->current_state](ctx);
        
        if (new_state >= STATE_COUNT || new_state < 0) {
            ctx->current_state = STATE_ERROR;
            ctx->error_code = 255;
            break;
        }
        
        ctx->current_state = new_state;
        
        if (old_state != new_state) {
            state_changes++;
        }
        
        if (ctx->current_state == STATE_ERROR) {
            break;
        }
        
        if (ctx->counter > UINT64_MAX - 1000) {
            ctx->current_state = STATE_ERROR;
            ctx->error_code = 254;
            break;
        }
        
        ctx->counter++;
    }
    
    printf("Final state: %d\n", ctx->current_state);
    printf("Iterations: %u\n", ctx->iterations);
    printf("State changes: %d\n", state_changes);
    printf("Final counter: %lu\n", ctx->counter);
    
    if (ctx->current_state == STATE_ERROR) {
        printf("Error code: %d\n", ctx->error_code);
    }
}

int main(void) {
    context_t ctx;
    
    ctx.current_state = STATE_IDLE;
    ctx.counter = 1;
    ctx.iterations = 0;
    ctx.error_code = 0;
    
    run_state_machine(&ctx);
    
    return ctx.current_state == STATE_ERROR ? EXIT_FAILURE : EXIT_SUCCESS;
}