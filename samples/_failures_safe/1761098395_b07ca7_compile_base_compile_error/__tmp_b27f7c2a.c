//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: state_machine
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

static state_t state_idle(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    ctx->counter = 0;
    memset(ctx->data, 0, sizeof(ctx->data));
    
    if (ctx->iterations > MAX_STEPS) {
        return STATE_ERROR;
    }
    
    return STATE_PROCESSING;
}

static state_t state_processing(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    for (int i = 0; i < 16; i++) {
        if (ctx->counter > UINT64_MAX - 1) {
            return STATE_ERROR;
        }
        ctx->counter += (ctx->data[i] * i) + 1;
    }
    
    if (ctx->counter % 1000 == 0) {
        return STATE_WAITING;
    }
    
    return STATE_VALIDATE;
}

static state_t state_waiting(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    uint64_t temp = ctx->counter;
    for (int i = 0; i < 100; i++) {
        if (temp > UINT64_MAX / 2) {
            temp = ctx->counter / 2;
        }
        temp = (temp * 3 + 1) / 2;
    }
    ctx->counter = temp;
    
    return STATE_PROCESSING;
}

static state_t state_validate(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    uint8_t checksum = 0;
    for (int i = 0; i < 16; i++) {
        checksum ^= ctx->data[i];
    }
    
    for (int i = 0; i < 16; i++) {
        ctx->data[i] = (ctx->data[i] + checksum + i) % 256;
    }
    
    if (ctx->iterations >= MAX_STEPS) {
        return STATE_COMPLETE;
    }
    
    ctx->iterations++;
    return STATE_PROCESSING;
}

static state_t state_complete(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    uint64_t result = 0;
    for (int i = 0; i < 16; i++) {
        if (result > UINT64_MAX - ctx->data[i]) {
            result = UINT64_MAX;
            break;
        }
        result += ctx->data[i];
    }
    
    printf("Final result: %lu\n", result);
    return STATE_FINALIZE;
}

static state_t state_error(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    printf("Error state reached\n");
    return STATE_RESET;
}

static state_t state_reset(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    ctx->current_state = STATE_IDLE;
    ctx->counter = 0;
    ctx->iterations = 0;
    memset(ctx->data, 0, sizeof(ctx->data));
    
    return STATE_IDLE;
}

static state_t state_finalize(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    uint64_t final_hash = ctx->counter;
    for (int i = 0; i < 16; i++) {
        final_hash = (final_hash * 31) + ctx->data[i];
    }
    
    printf("Final hash: %lu\n", final_hash);
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

int main(void) {
    context_t ctx;
    ctx.current_state = STATE_IDLE;
    ctx.counter = 0;
    ctx.iterations = 0;
    memset(ctx.data, 0, sizeof(ctx.data));
    
    srand((unsigned int)time(NULL));
    for (int i = 0; i < 16; i++) {
        ctx.data[i] = rand() % 256;
    }
    
    int step_count = 0;
    while (step_count < MAX_STEPS * 2) {
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
        
        if (ctx.current_state == STATE_IDLE && step_count > 1000) {
            break;
        }
        
        if (step_count >= MAX_STEPS * 2) {
            ctx.current_state = STATE_ERROR;
        }