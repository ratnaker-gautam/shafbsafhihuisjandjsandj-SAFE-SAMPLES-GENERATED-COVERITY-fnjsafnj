//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#define STATE_COUNT 8
#define MAX_STEPS 1000000

typedef enum {
    STATE_IDLE,
    STATE_PROCESSING,
    STATE_WAITING,
    STATE_COMPLETE,
    STATE_ERROR,
    STATE_RESET,
    STATE_VALIDATE,
    STATE_FINALIZE
} state_t;

typedef struct {
    state_t current_state;
    uint64_t counter;
    uint32_t iterations;
    uint8_t data[16];
} context_t;

static state_t handle_idle(context_t *ctx) {
    if (ctx->counter > 1000) {
        return STATE_PROCESSING;
    }
    ctx->counter += 17;
    return STATE_IDLE;
}

static state_t handle_processing(context_t *ctx) {
    if (ctx->iterations >= 100) {
        return STATE_VALIDATE;
    }
    for (int i = 0; i < 16; i++) {
        ctx->data[i] = (ctx->data[i] + ctx->counter + i) % 256;
    }
    ctx->iterations++;
    ctx->counter += 23;
    return STATE_PROCESSING;
}

static state_t handle_waiting(context_t *ctx) {
    if (ctx->counter % 100 == 0) {
        return STATE_PROCESSING;
    }
    ctx->counter += 7;
    return STATE_WAITING;
}

static state_t handle_complete(context_t *ctx) {
    return STATE_COMPLETE;
}

static state_t handle_error(context_t *ctx) {
    if (ctx->counter > 5000) {
        return STATE_RESET;
    }
    ctx->counter += 11;
    return STATE_ERROR;
}

static state_t handle_reset(context_t *ctx) {
    memset(ctx->data, 0, 16);
    ctx->iterations = 0;
    ctx->counter = 1;
    return STATE_IDLE;
}

static state_t handle_validate(context_t *ctx) {
    uint8_t sum = 0;
    for (int i = 0; i < 16; i++) {
        sum = (sum + ctx->data[i]) % 256;
    }
    if (sum % 2 == 0) {
        return STATE_FINALIZE;
    } else {
        return STATE_ERROR;
    }
}

static state_t handle_finalize(context_t *ctx) {
    for (int i = 0; i < 16; i++) {
        ctx->data[i] = (ctx->data[i] * 3 + 1) % 256;
    }
    return STATE_COMPLETE;
}

static state_t (*state_handlers[STATE_COUNT])(context_t *) = {
    handle_idle,
    handle_processing,
    handle_waiting,
    handle_complete,
    handle_error,
    handle_reset,
    handle_validate,
    handle_finalize
};

int main(void) {
    context_t ctx;
    ctx.current_state = STATE_IDLE;
    ctx.counter = 1;
    ctx.iterations = 0;
    memset(ctx.data, 0, 16);
    
    uint32_t step_count = 0;
    
    while (ctx.current_state != STATE_COMPLETE && step_count < MAX_STEPS) {
        if (ctx.current_state >= STATE_COUNT) {
            ctx.current_state = STATE_ERROR;
        }
        
        state_t next_state = state_handlers[ctx.current_state](&ctx);
        
        if (next_state >= STATE_COUNT) {
            ctx.current_state = STATE_ERROR;
        } else {
            ctx.current_state = next_state;
        }
        
        step_count++;
        
        if (ctx.counter > UINT64_MAX - 1000) {
            ctx.current_state = STATE_RESET;
        }
    }
    
    if (step_count >= MAX_STEPS) {
        return EXIT_FAILURE;
    }
    
    uint64_t result = 0;
    for (int i = 0; i < 16; i++) {
        result = (result * 31 + ctx.data[i]) % 1000000007;
    }
    
    printf("Final result: %lu\n", result);
    printf("Steps taken: %u\n", step_count);
    printf("Final counter: %lu\n", ctx.counter);
    
    return EXIT_SUCCESS;
}