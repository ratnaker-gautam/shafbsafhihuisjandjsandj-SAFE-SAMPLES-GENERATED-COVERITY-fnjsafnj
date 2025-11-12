//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define STATE_COUNT 5
#define MAX_ITERATIONS 1000000

typedef enum {
    STATE_INIT,
    STATE_PROCESSING,
    STATE_VALIDATION,
    STATE_COMPLETION,
    STATE_ERROR
} state_t;

typedef struct {
    state_t current_state;
    uint64_t counter;
    uint32_t iterations;
    int error_code;
} context_t;

static state_t handle_init(context_t *ctx) {
    if (ctx->counter > UINT64_MAX - 1000) {
        ctx->error_code = 1;
        return STATE_ERROR;
    }
    ctx->counter += 1000;
    return STATE_PROCESSING;
}

static state_t handle_processing(context_t *ctx) {
    if (ctx->counter > UINT64_MAX / 2) {
        ctx->error_code = 2;
        return STATE_ERROR;
    }
    ctx->counter *= 2;
    if (ctx->counter % 3 == 0) {
        return STATE_VALIDATION;
    }
    return STATE_PROCESSING;
}

static state_t handle_validation(context_t *ctx) {
    if (ctx->counter < 10000) {
        ctx->error_code = 3;
        return STATE_ERROR;
    }
    uint64_t check = ctx->counter;
    while (check > 10) {
        check /= 10;
    }
    if (check == 1) {
        return STATE_COMPLETION;
    }
    return STATE_PROCESSING;
}

static state_t handle_completion(context_t *ctx) {
    if (ctx->counter > UINT64_MAX - 1) {
        ctx->error_code = 4;
        return STATE_ERROR;
    }
    ctx->counter += 1;
    return STATE_COMPLETION;
}

static state_t handle_error(context_t *ctx) {
    return STATE_ERROR;
}

static state_t (*state_handlers[STATE_COUNT])(context_t *) = {
    handle_init,
    handle_processing,
    handle_validation,
    handle_completion,
    handle_error
};

static void run_state_machine(context_t *ctx) {
    while (ctx->iterations < MAX_ITERATIONS) {
        state_t next_state = state_handlers[ctx->current_state](ctx);
        if (next_state == ctx->current_state && next_state != STATE_ERROR) {
            ctx->iterations++;
            continue;
        }
        ctx->current_state = next_state;
        ctx->iterations++;
        if (ctx->current_state == STATE_COMPLETION || ctx->current_state == STATE_ERROR) {
            break;
        }
    }
}

int main(void) {
    context_t ctx;
    ctx.current_state = STATE_INIT;
    ctx.counter = 0;
    ctx.iterations = 0;
    ctx.error_code = 0;
    
    run_state_machine(&ctx);
    
    if (ctx.current_state == STATE_ERROR) {
        printf("State machine terminated with error code: %d\n", ctx.error_code);
        printf("Final counter value: %lu\n", ctx.counter);
        printf("Iterations: %u\n", ctx.iterations);
        return 1;
    } else if (ctx.current_state == STATE_COMPLETION) {
        printf("State machine completed successfully\n");
        printf("Final counter value: %lu\n", ctx.counter);
        printf("Iterations: %u\n", ctx.iterations);
        return 0;
    } else {
        printf("State machine reached maximum iterations\n");
        printf("Final counter value: %lu\n", ctx.counter);
        printf("Iterations: %u\n", ctx.iterations);
        return 2;
    }
}