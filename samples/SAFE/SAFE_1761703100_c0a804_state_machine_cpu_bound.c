//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define STATE_COUNT 8
#define MAX_ITERATIONS 1000000

typedef enum {
    STATE_IDLE,
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

static state_t handle_idle(context_t *ctx) {
    if (ctx->counter > UINT64_MAX - 1000) {
        return STATE_ERROR;
    }
    ctx->counter += 1;
    return STATE_PROCESSING;
}

static state_t handle_processing(context_t *ctx) {
    if (ctx->counter > UINT64_MAX - 10000) {
        return STATE_ERROR;
    }
    ctx->counter += ctx->counter / 2 + 1;
    return STATE_VALIDATING;
}

static state_t handle_validating(context_t *ctx) {
    if (ctx->counter % 2 == 0) {
        return STATE_COMPUTING;
    } else {
        return STATE_AGGREGATING;
    }
}

static state_t handle_computing(context_t *ctx) {
    if (ctx->counter > UINT64_MAX / 3) {
        return STATE_ERROR;
    }
    ctx->counter = ctx->counter * 3 + 1;
    return STATE_AGGREGATING;
}

static state_t handle_aggregating(context_t *ctx) {
    if (ctx->counter > UINT64_MAX - ctx->iterations) {
        return STATE_ERROR;
    }
    ctx->counter += ctx->iterations;
    return STATE_FINALIZING;
}

static state_t handle_finalizing(context_t *ctx) {
    if (ctx->counter > 1000000) {
        return STATE_DONE;
    } else {
        return STATE_IDLE;
    }
}

static state_t handle_error(context_t *ctx) {
    ctx->error_code = 1;
    return STATE_DONE;
}

static state_t handle_done(context_t *ctx) {
    return STATE_DONE;
}

static state_t (*state_handlers[STATE_COUNT])(context_t *) = {
    handle_idle,
    handle_processing,
    handle_validating,
    handle_computing,
    handle_aggregating,
    handle_finalizing,
    handle_error,
    handle_done
};

static void run_state_machine(context_t *ctx) {
    while (ctx->current_state != STATE_DONE && ctx->current_state != STATE_ERROR) {
        if (ctx->current_state >= STATE_COUNT) {
            ctx->current_state = STATE_ERROR;
            break;
        }
        state_t next_state = state_handlers[ctx->current_state](ctx);
        if (next_state >= STATE_COUNT) {
            ctx->current_state = STATE_ERROR;
            break;
        }
        ctx->current_state = next_state;
    }
    if (ctx->current_state == STATE_ERROR) {
        handle_error(ctx);
    }
}

int main(void) {
    context_t ctx;
    char input_buffer[64];
    
    printf("Enter number of iterations (1-1000000): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *endptr;
    long input_iterations = strtol(input_buffer, &endptr, 10);
    if (endptr == input_buffer || *endptr != '\n') {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    if (input_iterations < 1 || input_iterations > MAX_ITERATIONS) {
        fprintf(stderr, "Iterations out of range\n");
        return 1;
    }
    
    ctx.iterations = (uint32_t)input_iterations;
    ctx.counter = 1;
    ctx.current_state = STATE_IDLE;
    ctx.error_code = 0;
    
    run_state_machine(&ctx);
    
    if (ctx.error_code != 0) {
        printf("State machine terminated with error\n");
        return 1;
    }
    
    printf("Final counter value: %lu\n", ctx.counter);
    printf("Completed %u iterations\n", ctx.iterations);
    
    return 0;
}