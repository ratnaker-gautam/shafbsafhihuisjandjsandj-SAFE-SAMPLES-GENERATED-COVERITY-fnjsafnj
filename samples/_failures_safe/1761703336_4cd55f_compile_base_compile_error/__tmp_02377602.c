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
    for (int i = 0; i < 16; i++) {
        ctx->data[i] = (uint8_t)(i * 7 + 3);
    }
    
    if (ctx->steps >= MAX_STEPS) {
        return STATE_COMPLETE;
    }
    
    return STATE_PROCESSING;
}

static state_t state_processing(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    for (int i = 0; i < 16; i++) {
        if (ctx->data[i] > 200) {
            return STATE_ERROR;
        }
        ctx->data[i] = (ctx->data[i] * 3 + 1) % 251;
    }
    
    ctx->counter++;
    ctx->steps++;
    
    if (ctx->counter % 100 == 0) {
        return STATE_VALIDATE;
    }
    
    if (ctx->counter >= 5000) {
        return STATE_COMPLETE;
    }
    
    return STATE_WAITING;
}

static state_t state_waiting(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    uint64_t temp = 0;
    for (int i = 0; i < 16; i++) {
        temp += ctx->data[i];
    }
    
    if (temp % 7 == 0) {
        return STATE_PROCESSING;
    }
    
    ctx->steps++;
    return STATE_RESET;
}

static state_t state_complete(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    return STATE_FINALIZE;
}

static state_t state_error(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    for (int i = 0; i < 16; i++) {
        ctx->data[i] = 0xFF;
    }
    
    ctx->steps++;
    return STATE_RESET;
}

static state_t state_reset(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    ctx->counter = 0;
    for (int i = 0; i < 16; i++) {
        ctx->data[i] = (uint8_t)(i * 11 + 5);
    }
    
    ctx->steps++;
    return STATE_IDLE;
}

static state_t state_validate(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    uint8_t sum = 0;
    for (int i = 0; i < 16; i++) {
        sum += ctx->data[i];
    }
    
    if (sum % 2 == 0) {
        ctx->steps++;
        return STATE_PROCESSING;
    } else {
        ctx->steps++;
        return STATE_RESET;
    }
}

static state_t state_finalize(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    return STATE_COMPLETE;
}

int main(void) {
    context_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    ctx.current_state = STATE_IDLE;
    ctx.steps = 0;
    
    printf("Starting state machine simulation...\n");
    
    while (ctx.current_state != STATE_COMPLETE && ctx.current_state != STATE_FINALIZE) {
        if (ctx.steps >= MAX_STEPS) {
            printf("Maximum steps exceeded\n");
            break;
        }
        
        if (ctx.current_state >= STATE_COUNT) {
            printf("Invalid state detected\n");
            break;
        }
        
        state_t next_state = state_handlers[ctx.current_state](&ctx);
        
        if (next_state >= STATE_COUNT) {
            printf("Invalid next state\n");
            break;
        }
        
        ctx.current_state = next_state;
        
        if (ctx.steps % 10000 == 0 && ctx.steps > 0) {
            printf("Progress: %u steps, counter: %lu\n", ctx.steps, ctx.counter);
        }
    }
    
    printf("Final state: %d\n", ctx.current_state);
    printf("Total steps: %u\n