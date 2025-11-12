//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: state_machine
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
    uint32_t iterations;
    int error_code;
} context_t;

static state_t state_init(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    ctx->counter = 0;
    ctx->iterations = 0;
    ctx->error_code = 0;
    return STATE_PROCESSING;
}

static state_t state_processing(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    for (int i = 0; i < 1000; i++) {
        if (ctx->counter > UINT64_MAX - 1000) return STATE_ERROR;
        ctx->counter += i * 3;
    }
    return STATE_VALIDATING;
}

static state_t state_validating(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    if (ctx->counter % 2 == 0) {
        return STATE_COMPUTING;
    } else {
        ctx->counter *= 2;
        return STATE_AGGREGATING;
    }
}

static state_t state_computing(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    uint64_t temp = 0;
    for (int i = 0; i < 500; i++) {
        if (temp > UINT64_MAX - (i * 7)) return STATE_ERROR;
        temp += i * 7;
    }
    if (ctx->counter > UINT64_MAX - temp) return STATE_ERROR;
    ctx->counter += temp;
    return STATE_AGGREGATING;
}

static state_t state_aggregating(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    uint64_t sum = 0;
    for (int i = 0; i < 100; i++) {
        if (sum > UINT64_MAX - (ctx->counter % 1000)) return STATE_ERROR;
        sum += ctx->counter % 1000;
    }
    if (ctx->counter > UINT64_MAX - sum) return STATE_ERROR;
    ctx->counter += sum;
    return STATE_FINALIZING;
}

static state_t state_finalizing(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    ctx->counter = (ctx->counter * 13) % 1000000;
    ctx->iterations++;
    if (ctx->iterations >= 100) {
        return STATE_DONE;
    }
    return STATE_PROCESSING;
}

static state_t state_error(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    ctx->error_code = 1;
    return STATE_DONE;
}

static state_t state_done(context_t *ctx) {
    return STATE_DONE;
}

typedef state_t (*state_handler_t)(context_t*);

static state_handler_t state_handlers[STATE_COUNT] = {
    state_init,
    state_processing,
    state_validating,
    state_computing,
    state_aggregating,
    state_finalizing,
    state_error,
    state_done
};

int main(void) {
    context_t ctx;
    ctx.current_state = STATE_INIT;
    ctx.counter = 0;
    ctx.iterations = 0;
    ctx.error_code = 0;

    uint32_t step_count = 0;
    
    while (ctx.current_state != STATE_DONE && step_count < MAX_ITERATIONS) {
        if (ctx.current_state >= STATE_COUNT) {
            ctx.current_state = STATE_ERROR;
        }
        
        state_handler_t handler = state_handlers[ctx.current_state];
        if (handler == NULL) {
            ctx.current_state = STATE_ERROR;
            continue;
        }
        
        state_t next_state = handler(&ctx);
        if (next_state >= STATE_COUNT) {
            ctx.current_state = STATE_ERROR;
        } else {
            ctx.current_state = next_state;
        }
        
        step_count++;
        if (step_count >= MAX_ITERATIONS) {
            ctx.current_state = STATE_ERROR;
        }
    }

    if (ctx.error_code != 0) {
        printf("State machine terminated with error after %u steps\n", step_count);
        return 1;
    } else {
        printf("State machine completed successfully after %u steps\n", step_count);
        printf("Final counter value: %lu\n", (unsigned long)ctx.counter);
        return 0;
    }
}