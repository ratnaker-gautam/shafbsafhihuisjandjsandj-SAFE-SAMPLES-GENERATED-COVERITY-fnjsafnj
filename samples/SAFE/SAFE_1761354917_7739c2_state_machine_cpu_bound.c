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

static state_t handle_idle(context_t *ctx) {
    if (ctx->counter % 3 == 0) {
        return STATE_PROCESSING;
    } else if (ctx->counter % 7 == 0) {
        return STATE_VALIDATE;
    }
    return STATE_IDLE;
}

static state_t handle_processing(context_t *ctx) {
    for (int i = 0; i < 16; i++) {
        ctx->data[i] = (ctx->data[i] + ctx->counter) % 256;
    }
    return STATE_WAITING;
}

static state_t handle_waiting(context_t *ctx) {
    if (ctx->counter % 5 == 0) {
        return STATE_COMPLETE;
    }
    return STATE_PROCESSING;
}

static state_t handle_complete(context_t *ctx) {
    uint8_t sum = 0;
    for (int i = 0; i < 16; i++) {
        sum += ctx->data[i];
    }
    if (sum % 2 == 0) {
        return STATE_FINALIZE;
    }
    return STATE_ERROR;
}

static state_t handle_error(context_t *ctx) {
    memset(ctx->data, 0, 16);
    return STATE_RESET;
}

static state_t handle_reset(context_t *ctx) {
    ctx->counter = 0;
    return STATE_IDLE;
}

static state_t handle_validate(context_t *ctx) {
    for (int i = 0; i < 16; i++) {
        if (ctx->data[i] > 200) {
            return STATE_ERROR;
        }
    }
    return STATE_IDLE;
}

static state_t handle_finalize(context_t *ctx) {
    uint64_t result = 0;
    for (int i = 0; i < 16; i++) {
        result += ctx->data[i];
    }
    ctx->counter = result;
    return STATE_IDLE;
}

static void run_state_machine(context_t *ctx) {
    state_t (*handlers[STATE_COUNT])(context_t *) = {
        handle_idle,
        handle_processing,
        handle_waiting,
        handle_complete,
        handle_error,
        handle_reset,
        handle_validate,
        handle_finalize
    };

    while (ctx->steps < MAX_STEPS) {
        if (ctx->current_state >= STATE_COUNT) {
            ctx->current_state = STATE_ERROR;
        }
        
        state_t next_state = handlers[ctx->current_state](ctx);
        ctx->current_state = next_state;
        ctx->counter++;
        ctx->steps++;

        if (ctx->current_state == STATE_IDLE && ctx->counter % 100000 == 0) {
            break;
        }
    }
}

int main(void) {
    context_t ctx;
    ctx.current_state = STATE_IDLE;
    ctx.counter = 1;
    ctx.steps = 0;
    
    for (int i = 0; i < 16; i++) {
        ctx.data[i] = (i * 13 + 7) % 256;
    }

    run_state_machine(&ctx);

    uint64_t final_sum = 0;
    for (int i = 0; i < 16; i++) {
        final_sum += ctx.data[i];
    }

    printf("State machine completed after %u steps\n", ctx.steps);
    printf("Final counter: %lu\n", ctx.counter);
    printf("Final data sum: %lu\n", final_sum);

    return 0;
}