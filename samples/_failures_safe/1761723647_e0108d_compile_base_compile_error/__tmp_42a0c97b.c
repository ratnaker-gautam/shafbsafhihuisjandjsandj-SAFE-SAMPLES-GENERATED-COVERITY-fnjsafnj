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

static state_t state_idle(context_t *ctx);
static state_t state_processing(context_t *ctx);
static state_t state_waiting(context_t *ctx);
static state_t state_complete(context_t *ctx);
static state_t state_error(context_t *ctx);
static state_t state_reset(context_t *ctx);
static state_t state_validate(context_t *ctx);
static state_t state_finalize(context_t *ctx);

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

static state_t state_idle(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    ctx->counter = 0;
    memset(ctx->data, 0, sizeof(ctx->data));
    
    for (int i = 0; i < 16; i++) {
        ctx->data[i] = (uint8_t)((i * 7 + 11) % 256);
    }
    
    return STATE_PROCESSING;
}

static state_t state_processing(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    if (ctx->counter > UINT64_MAX - 1000) {
        return STATE_ERROR;
    }
    
    for (int i = 0; i < 1000; i++) {
        for (int j = 0; j < 16; j++) {
            ctx->data[j] = (ctx->data[j] + ctx->counter + i + j) % 256;
        }
        ctx->counter++;
    }
    
    if (ctx->counter % 5000 == 0) {
        return STATE_VALIDATE;
    }
    
    return STATE_PROCESSING;
}

static state_t state_waiting(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    uint32_t wait_cycles = 100 + (ctx->counter % 100);
    for (uint32_t i = 0; i < wait_cycles; i++) {
        ctx->data[i % 16] = (ctx->data[i % 16] + i) % 256;
    }
    
    return STATE_PROCESSING;
}

static state_t state_complete(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    uint64_t checksum = 0;
    for (int i = 0; i < 16; i++) {
        checksum += ctx->data[i];
    }
    
    return STATE_FINALIZE;
}

static state_t state_error(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    memset(ctx->data, 0xFF, sizeof(ctx->data));
    return STATE_RESET;
}

static state_t state_reset(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    ctx->counter = 0;
    memset(ctx->data, 0, sizeof(ctx->data));
    ctx->steps = 0;
    
    return STATE_IDLE;
}

static state_t state_validate(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    uint8_t valid = 1;
    for (int i = 0; i < 16; i++) {
        if (ctx->data[i] == 0 || ctx->data[i] == 0xFF) {
            valid = 0;
            break;
        }
    }
    
    if (valid) {
        return STATE_PROCESSING;
    } else {
        return STATE_ERROR;
    }
}

static state_t state_finalize(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 8; j++) {
            ctx->data[i] ^= (1 << j);
        }
    }
    
    return STATE_COMPLETE;
}

int main(void) {
    context_t ctx;
    ctx.current_state = STATE_IDLE;
    ctx.counter = 0;
    ctx.steps = 0;
    memset(ctx.data, 0, sizeof(ctx.data));
    
    printf("Starting state machine simulation\n");
    
    while (ctx.steps < MAX_STEPS) {
        if (ctx.current_state >= STATE_COUNT) {
            printf("Invalid state detected\n");
            return 1;
        }
        
        state_t next_state = state_handlers[ctx.current_state](&ctx);
        ctx.current_state = next_state;
        ctx.steps++;
        
        if (ctx.current_state == STATE_COMPLETE) {
            printf("Simulation completed successfully after %u steps\n", ctx.steps);
            break;
        }
        
        if (ctx.steps >= MAX_STEPS) {
            printf("Maximum steps reached\n");
            break