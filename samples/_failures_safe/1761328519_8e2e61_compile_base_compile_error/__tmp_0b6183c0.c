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
    uint32_t steps;
    uint8_t data[16];
} context_t;

static state_t state_idle(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    ctx->counter += 1;
    for (int i = 0; i < 16; i++) {
        ctx->data[i] = (uint8_t)((ctx->counter + i) & 0xFF);
    }
    
    if (ctx->counter % 3 == 0) return STATE_PROCESSING;
    if (ctx->counter % 7 == 0) return STATE_VALIDATE;
    return STATE_IDLE;
}

static state_t state_processing(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    uint32_t sum = 0;
    for (int i = 0; i < 16; i++) {
        sum += ctx->data[i];
    }
    
    for (int i = 0; i < 16; i++) {
        ctx->data[i] = (uint8_t)((ctx->data[i] + sum + i) & 0xFF);
    }
    
    if (sum % 5 == 0) return STATE_WAITING;
    if (sum % 11 == 0) return STATE_VALIDATE;
    return STATE_PROCESSING;
}

static state_t state_waiting(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    ctx->counter += 100;
    
    uint32_t product = 1;
    for (int i = 0; i < 8; i++) {
        product *= (ctx->data[i] + 1);
        if (product == 0) product = 1;
    }
    
    if (product % 13 == 0) return STATE_COMPLETE;
    if (product % 17 == 0) return STATE_RESET;
    return STATE_WAITING;
}

static state_t state_validate(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    uint8_t checksum = 0;
    for (int i = 0; i < 16; i++) {
        checksum ^= ctx->data[i];
    }
    
    for (int i = 0; i < 16; i++) {
        ctx->data[i] = (uint8_t)((ctx->data[i] + checksum + i * 7) & 0xFF);
    }
    
    if (checksum % 3 == 0) return STATE_PROCESSING;
    if (checksum % 7 == 0) return STATE_FINALIZE;
    return STATE_VALIDATE;
}

static state_t state_finalize(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    uint64_t total = 0;
    for (int i = 0; i < 16; i++) {
        total += ctx->data[i] * (i + 1);
    }
    ctx->counter += total;
    
    if (total % 19 == 0) return STATE_COMPLETE;
    return STATE_FINALIZE;
}

static state_t state_complete(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    return STATE_COMPLETE;
}

static state_t state_error(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    for (int i = 0; i < 16; i++) {
        ctx->data[i] = 0xFF;
    }
    return STATE_ERROR;
}

static state_t state_reset(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    ctx->counter = 0;
    memset(ctx->data, 0, sizeof(ctx->data));
    return STATE_IDLE;
}

static state_t (*state_handlers[STATE_COUNT])(context_t *) = {
    state_idle,
    state_processing,
    state_waiting,
    state_complete,
    state_error,
    state_reset,
    state_validate,
    state_finalize
};

static void run_state_machine(uint32_t max_iterations) {
    if (max_iterations == 0 || max_iterations > MAX_STEPS) {
        max_iterations = 1000;
    }
    
    context_t ctx;
    ctx.current_state = STATE_IDLE;
    ctx.counter = 0;
    ctx.steps = 0;
    memset(ctx.data, 0, sizeof(ctx.data));
    
    printf("Starting state machine with %u iterations\n", max_iterations);
    
    while (ctx.steps < max_iterations) {
        if (ctx.current_state >= STATE_COUNT) {
            ctx.current_state = STATE_ERROR;
        }
        
        state_t next_state = state_handlers[ctx.current_state](&ctx);
        ctx.current_state = next_state;
        ctx.steps++;
        
        if (ctx.current_state == STATE_COMPLETE || ctx.current_state == STATE_ERROR) {
            break;
        }
    }