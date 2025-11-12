//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define STATE_COUNT 5
#define MAX_ITERATIONS 1000000

typedef enum {
    STATE_START,
    STATE_PROCESSING,
    STATE_VALIDATION,
    STATE_FINALIZE,
    STATE_TERMINATE
} state_t;

typedef struct {
    state_t current_state;
    uint64_t counter;
    uint32_t iterations;
} context_t;

static state_t handle_start(context_t *ctx) {
    if (ctx == NULL) return STATE_TERMINATE;
    ctx->counter = 0;
    return STATE_PROCESSING;
}

static state_t handle_processing(context_t *ctx) {
    if (ctx == NULL) return STATE_TERMINATE;
    if (ctx->counter > UINT64_MAX - ctx->iterations) {
        return STATE_TERMINATE;
    }
    ctx->counter += ctx->iterations;
    return STATE_VALIDATION;
}

static state_t handle_validation(context_t *ctx) {
    if (ctx == NULL) return STATE_TERMINATE;
    if (ctx->counter % 2 == 0) {
        return STATE_FINALIZE;
    } else {
        ctx->counter += 1;
        return STATE_PROCESSING;
    }
}

static state_t handle_finalize(context_t *ctx) {
    if (ctx == NULL) return STATE_TERMINATE;
    if (ctx->counter > 0) {
        ctx->counter = (ctx->counter * 1103515245 + 12345) % 2147483648;
    }
    return STATE_TERMINATE;
}

static state_t handle_terminate(context_t *ctx) {
    return STATE_TERMINATE;
}

static state_t (*state_handlers[STATE_COUNT])(context_t *) = {
    handle_start,
    handle_processing,
    handle_validation,
    handle_finalize,
    handle_terminate
};

static void run_state_machine(context_t *ctx) {
    if (ctx == NULL) return;
    
    while (ctx->current_state != STATE_TERMINATE) {
        if (ctx->current_state < 0 || ctx->current_state >= STATE_COUNT) {
            break;
        }
        ctx->current_state = state_handlers[ctx->current_state](ctx);
    }
}

int main(void) {
    context_t ctx;
    char input_buffer[32];
    char *endptr;
    long input_value;
    
    printf("Enter number of iterations (1-1000000): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    input_value = strtol(input_buffer, &endptr, 10);
    if (endptr == input_buffer || *endptr != '\n') {
        fprintf(stderr, "Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    if (input_value < 1 || input_value > MAX_ITERATIONS) {
        fprintf(stderr, "Input out of range\n");
        return EXIT_FAILURE;
    }
    
    ctx.iterations = (uint32_t)input_value;
    ctx.current_state = STATE_START;
    ctx.counter = 0;
    
    run_state_machine(&ctx);
    
    printf("Final counter value: %lu\n", ctx.counter);
    return EXIT_SUCCESS;
}