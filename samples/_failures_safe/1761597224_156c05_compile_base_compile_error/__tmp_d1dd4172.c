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

static state_t state_init(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    ctx->counter = 0;
    for (int i = 0; i < STATE_COUNT; i++) {
        ctx->data[i] = 0;
    }
    ctx->iteration = 0;
    
    return STATE_PROCESSING;
}

static state_t state_processing(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    for (int i = 0; i < 1000; i++) {
        if (ctx->counter > UINT64_MAX - i) {
            return STATE_ERROR;
        }
        ctx->counter += i;
    }
    
    ctx->data[1] = (uint32_t)(ctx->counter % UINT32_MAX);
    return STATE_VALIDATING;
}

static state_t state_validating(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    if (ctx->data[1] == 0) {
        ctx->data[1] = 1;
    }
    
    for (int i = 0; i < 500; i++) {
        uint32_t product = ctx->data[1] * i;
        if (ctx->data[2] > UINT32_MAX - product) {
            return STATE_ERROR;
        }
        ctx->data[2] += product;
    }
    
    return STATE_COMPUTING;
}

static state_t state_computing(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    uint64_t temp = 0;
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            uint64_t add_value = (uint64_t)i * j + ctx->data[2];
            if (temp > UINT64_MAX - add_value) {
                return STATE_ERROR;
            }
            temp += add_value;
        }
    }
    
    ctx->data[3] = (uint32_t)(temp % UINT32_MAX);
    return STATE_AGGREGATING;
}

static state_t state_aggregating(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    ctx->data[4] = 0;
    for (int i = 0; i < STATE_COUNT; i++) {
        if (ctx->data[4] > UINT32_MAX - ctx->data[i]) {
            return STATE_ERROR;
        }
        ctx->data[4] += ctx->data[i];
    }
    
    return STATE_FINALIZING;
}

static state_t state_finalizing(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    if (ctx->data[4] > UINT32_MAX / 2) {
        return STATE_ERROR;
    }
    ctx->data[5] = ctx->data[4] * 2;
    
    ctx->iteration++;
    if (ctx->iteration >= 100) {
        return STATE_DONE;
    }
    
    return STATE_PROCESSING;
}

static state_t state_error(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    for (int i = 0; i < STATE_COUNT; i++) {
        ctx->data[i] = 0;
    }
    ctx->counter = 0;
    return STATE_DONE;
}

static state_t state_done(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    return STATE_DONE;
}

static state_t (*state_handlers[STATE_COUNT])(context_t *) = {
    state_init,
    state_processing,
    state_validating,
    state_computing,
    state_aggregating,
    state_finalizing,
    state_error,
    state_done
};

static void run_state_machine(context_t *ctx) {
    if (ctx == NULL) return;
    
    int iterations = 0;
    
    while (ctx->current_state != STATE_DONE && iterations < MAX_ITERATIONS) {
        if (ctx->current_state < 0 || ctx->current_state >= STATE_COUNT) {
            ctx->current_state = STATE_ERROR;
        }
        
        state_t next_state = state_handlers[ctx->current_state](ctx);
        
        if (next_state < 0 || next_state >= STATE_COUNT) {
            ctx->current_state = STATE_ERROR;
        } else {
            ctx->current_state = next_state;
        }
        
        iterations++;
        
        if (iterations >= MAX_ITERATIONS) {
            ctx->current_state = STATE_ERROR;
        }
    }
}

int main(void) {
    context_t ctx;
    
    ctx.current_state = STATE_INIT;
    
    run_state_machine(&ctx);
    
    uint64_t result = 0;
    for (int i = 0; i < STATE_COUNT; i++) {
        if (result