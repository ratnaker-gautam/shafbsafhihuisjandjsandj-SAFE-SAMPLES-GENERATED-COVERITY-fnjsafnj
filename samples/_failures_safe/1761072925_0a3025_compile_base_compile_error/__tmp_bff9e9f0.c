//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

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
    
    if (ctx->counter < UINT64_MAX - 1) {
        ctx->counter += 1;
    }
    memset(ctx->data, 0, sizeof(ctx->data));
    
    if (ctx->counter % 3 == 0) {
        return STATE_PROCESSING;
    } else if (ctx->counter % 7 == 0) {
        return STATE_VALIDATE;
    }
    
    return STATE_IDLE;
}

static state_t state_processing(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    for (int i = 0; i < 16; i++) {
        ctx->data[i] = (uint8_t)((ctx->counter + i) % 256);
    }
    
    uint32_t sum = 0;
    for (int i = 0; i < 16; i++) {
        sum += ctx->data[i];
    }
    
    if (sum % 2 == 0) {
        return STATE_WAITING;
    } else {
        return STATE_VALIDATE;
    }
}

static state_t state_waiting(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    if (ctx->counter <= UINT64_MAX - 7) {
        ctx->counter += 7;
    }
    
    uint32_t check = 0;
    for (int i = 0; i < 8; i++) {
        check += ctx->data[i] * ctx->data[15 - i];
    }
    
    if (check % 5 == 0) {
        return STATE_COMPLETE;
    } else {
        return STATE_PROCESSING;
    }
}

static state_t state_complete(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    uint64_t result = 0;
    for (int i = 0; i < 16; i++) {
        if (result > UINT64_MAX >> 4) {
            result = UINT64_MAX;
            break;
        }
        result = (result << 4) | (ctx->data[i] & 0x0F);
    }
    
    if (result > ctx->counter) {
        return STATE_FINALIZE;
    } else {
        return STATE_RESET;
    }
}

static state_t state_error(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    ctx->counter = 0;
    memset(ctx->data, 0xFF, sizeof(ctx->data));
    return STATE_RESET;
}

static state_t state_reset(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    ctx->counter = 0;
    memset(ctx->data, 0, sizeof(ctx->data));
    return STATE_IDLE;
}

static state_t state_validate(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    uint8_t valid = 1;
    for (int i = 0; i < 16; i++) {
        if (ctx->data[i] > 200) {
            valid = 0;
            break;
        }
    }
    
    if (valid && (ctx->counter % 11 == 0)) {
        return STATE_COMPLETE;
    } else {
        return STATE_PROCESSING;
    }
}

static state_t state_finalize(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    uint64_t final_value = 0;
    for (int i = 0; i < 16; i++) {
        if (final_value > UINT64_MAX - (ctx->data[i] * (i + 1))) {
            final_value = UINT64_MAX;
            break;
        }
        final_value += ctx->data[i] * (i + 1);
    }
    
    ctx->counter = final_value;
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
    ctx.steps = 0;
    memset(ctx.data, 0, sizeof(ctx.data));
    
    printf("Starting state machine simulation\n");
    
    while (ctx.steps < MAX_STEPS) {
        if (ctx.current_state >= STATE_COUNT) {
            ctx.current_state = STATE_ERROR;
        }
        
        state_t next_state = state_handlers[ctx.current_state](&ctx);
        
        if (next_state >= STATE_COUNT) {
            ctx.current_state = STATE_ERROR;
        } else {
            ctx.current_state = next_state;
        }