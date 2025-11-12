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
    uint8_t data_buffer[64];
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
    for (int i = 0; i < 64; i++) {
        ctx->data_buffer[i] = (uint8_t)((ctx->counter + i) % 256);
    }
    
    uint32_t sum = 0;
    for (int i = 0; i < 64; i++) {
        sum += ctx->data_buffer[i];
    }
    
    if (sum % 2 == 0) {
        return STATE_WAITING;
    } else {
        return STATE_COMPLETING;
    }
}

static state_t handle_waiting(context_t *ctx) {
    if (ctx->counter % 5 == 0) {
        return STATE_PROCESSING;
    }
    return STATE_COMPLETING;
}

static state_t handle_completing(context_t *ctx) {
    uint32_t check = 0;
    for (int i = 0; i < 32; i++) {
        check ^= ctx->data_buffer[i];
    }
    
    if (check == 0) {
        return STATE_ERROR;
    } else if (ctx->counter % 11 == 0) {
        return STATE_FINAL;
    }
    return STATE_IDLE;
}

static state_t handle_error(context_t *ctx) {
    memset(ctx->data_buffer, 0, 64);
    return STATE_RESET;
}

static state_t handle_reset(context_t *ctx) {
    ctx->counter = 0;
    return STATE_IDLE;
}

static state_t handle_validating(context_t *ctx) {
    for (int i = 0; i < 64; i++) {
        if (ctx->data_buffer[i] > 200) {
            return STATE_ERROR;
        }
    }
    return STATE_IDLE;
}

static state_t handle_final(context_t *ctx) {
    return STATE_FINAL;
}

static void run_state_machine(context_t *ctx) {
    typedef state_t (*state_handler_t)(context_t*);
    
    static const state_handler_t handlers[STATE_COUNT] = {
        handle_idle,
        handle_processing,
        handle_waiting,
        handle_completing,
        handle_error,
        handle_reset,
        handle_validating,
        handle_final
    };
    
    while (ctx->iterations < MAX_ITERATIONS) {
        if (ctx->current_state >= STATE_COUNT) {
            ctx->current_state = STATE_ERROR;
        }
        
        state_t next_state = handlers[ctx->current_state](ctx);
        
        if (next_state >= STATE_COUNT) {
            next_state = STATE_ERROR;
        }
        
        ctx->current_state = next_state;
        ctx->counter++;
        ctx->iterations++;
        
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
    memset(ctx.data_buffer, 0, 64);
    
    run_state_machine(&ctx);
    
    printf("State machine completed after %u iterations\n", ctx.iterations);
    printf("Final counter value: %lu\n", (unsigned long)ctx.counter);
    printf("Final state: %d\n", ctx.current_state);
    
    uint32_t final_sum = 0;
    for (int i = 0; i < 64; i++) {
        final_sum += ctx.data_buffer[i];
    }
    printf("Final buffer sum: %u\n", final_sum);
    
    return 0;
}