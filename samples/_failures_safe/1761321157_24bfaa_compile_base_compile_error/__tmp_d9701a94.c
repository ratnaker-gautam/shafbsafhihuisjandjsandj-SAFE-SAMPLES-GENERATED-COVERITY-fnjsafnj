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
        if (i > 0 && ctx->counter > UINT64_MAX / i) {
            ctx->counter = 0;
        }
        uint64_t add_val = (uint64_t)i * (uint64_t)ctx->iteration;
        if (ctx->counter > UINT64_MAX - add_val) {
            ctx->counter = 0;
        } else {
            ctx->counter += add_val;
        }
    }
    
    ctx->data[1] = (ctx->counter % 1000) + 1;
    return STATE_VALIDATING;
}

static state_t state_validating(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    if (ctx->data[1] == 0 || ctx->data[1] > 1000) {
        return STATE_ERROR;
    }
    
    for (int i = 0; i < 500; i++) {
        uint32_t add_val = (ctx->data[1] * i) % 100;
        if (ctx->data[2] > UINT32_MAX - add_val) {
            ctx->data[2] = ctx->data[2] % 1000;
        } else {
            ctx->data[2] += add_val;
        }
        if (ctx->data[2] > 10000) {
            ctx->data[2] = ctx->data[2] % 1000;
        }
    }
    
    return STATE_COMPUTING;
}

static state_t state_computing(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    uint32_t temp = 0;
    for (int i = 0; i < 2000; i++) {
        if (ctx->data[1] > UINT32_MAX - ctx->data[2] || 
            ctx->data[1] + ctx->data[2] > UINT32_MAX - i) {
            temp = 500;
        } else {
            temp = (ctx->data[1] + ctx->data[2] + i) * 3;
        }
        uint32_t mod_val = temp % 500;
        if (ctx->data[3] > UINT32_MAX - mod_val) {
            ctx->data[3] = ctx->data[3] % 1000;
        } else {
            ctx->data[3] += mod_val;
        }
        if (ctx->data[3] > 2000) {
            ctx->data[3] = ctx->data[3] % 1000;
        }
    }
    
    return STATE_AGGREGATING;
}

static state_t state_aggregating(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    for (int i = 0; i < STATE_COUNT; i++) {
        if (ctx->data[4] > UINT32_MAX - ctx->data[i]) {
            ctx->data[4] = ctx->data[4] % 50000;
        } else {
            ctx->data[4] += ctx->data[i];
        }
        if (ctx->data[4] > 100000) {
            ctx->data[4] = ctx->data[4] % 50000;
        }
    }
    
    if (ctx->iteration > 0 && ctx->data[4] > UINT32_MAX / ctx->iteration) {
        ctx->data[5] = 999;
    } else {
        ctx->data[5] = (ctx->data[4] * ctx->iteration) % 1000;
    }
    return STATE_FINALIZING;
}

static state_t state_finalizing(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    ctx->data[6] = 0;
    for (int i = 0; i < 1000; i++) {
        uint32_t add_val = (ctx->data[5] + i) % 100;
        if (ctx->data[6] > UINT32_MAX - add_val) {
            ctx->data[6] = ctx->data[6] % 2500;
        } else {
            ctx->data[6] += add_val;
        }
        if (ctx->data[6] > 5000) {
            ctx->data[6] = ctx->data[6] %