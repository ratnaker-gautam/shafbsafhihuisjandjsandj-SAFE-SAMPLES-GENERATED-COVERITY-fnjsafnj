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
    if (ctx->counter % 3 == 0) {
        return STATE_PROCESSING;
    } else if (ctx->counter % 7 == 0) {
        return STATE_WAITING;
    }
    ctx->counter++;
    return STATE_IDLE;
}

static state_t state_processing(context_t *ctx) {
    for (int i = 0; i < 16; i++) {
        ctx->data[i] = (ctx->data[i] + ctx->counter) % 256;
    }
    ctx->counter++;
    if (ctx->counter % 5 == 0) {
        return STATE_VALIDATE;
    }
    return STATE_PROCESSING;
}

static state_t state_waiting(context_t *ctx) {
    ctx->counter += 2;
    if (ctx->counter % 11 == 0) {
        return STATE_PROCESSING;
    }
    return STATE_WAITING;
}

static state_t state_validate(context_t *ctx) {
    uint8_t sum = 0;
    for (int i = 0; i < 16; i++) {
        sum += ctx->data[i];
    }
    if (sum % 2 == 0) {
        return STATE_FINALIZE;
    } else {
        return STATE_RESET;
    }
}

static state_t state_reset(context_t *ctx) {
    memset(ctx->data, 0, 16);
    ctx->counter = 0;
    return STATE_IDLE;
}

static state_t state_finalize(context_t *ctx) {
    for (int i = 0; i < 16; i++) {
        ctx->data[i] = ctx->data[i] ^ 0xFF;
    }
    return STATE_COMPLETE;
}

static state_t state_complete(context_t *ctx) {
    return STATE_COMPLETE;
}

static state_t state_error(context_t *ctx) {
    return STATE_ERROR;
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

static void run_state_machine(context_t *ctx) {
    while (ctx->steps < MAX_STEPS) {
        state_t next_state = state_handlers[ctx->current_state](ctx);
        if (next_state == ctx->current_state && 
            (next_state == STATE_COMPLETE || next_state == STATE_ERROR)) {
            break;
        }
        ctx->current_state = next_state;
        ctx->steps++;
        
        if (ctx->steps % 100000 == 0) {
            uint8_t checksum = 0;
            for (int i = 0; i < 16; i++) {
                checksum ^= ctx->data[i];
            }
            printf("Step: %u, State: %d, Checksum: %u\n", 
                   ctx->steps, ctx->current_state, checksum);
        }
    }
}

int main(void) {
    context_t ctx;
    
    ctx.current_state = STATE_IDLE;
    ctx.counter = 1;
    ctx.steps = 0;
    memset(ctx.data, 0xAA, 16);
    
    run_state_machine(&ctx);
    
    printf("Final state: %d\n", ctx.current_state);
    printf("Total steps: %u\n", ctx.steps);
    printf("Final counter: %lu\n", ctx.counter);
    
    uint8_t final_checksum = 0;
    for (int i = 0; i < 16; i++) {
        final_checksum ^= ctx.data[i];
    }
    printf("Final data checksum: %u\n", final_checksum);
    
    return 0;
}