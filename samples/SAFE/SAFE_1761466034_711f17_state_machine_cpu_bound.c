//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

enum state {
    STATE_IDLE,
    STATE_PROCESSING,
    STATE_COMPLETED,
    STATE_ERROR,
    STATE_COUNT
};

struct context {
    enum state current_state;
    uint32_t counter;
    uint32_t max_iterations;
    uint32_t error_threshold;
};

static int validate_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > 10) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] < '0' || input[i] > '9') return 0;
    }
    return 1;
}

static uint32_t parse_uint32(const char *input) {
    unsigned long long val = 0;
    for (size_t i = 0; input[i] != '\0'; i++) {
        if (val > UINT32_MAX / 10) return UINT32_MAX;
        val = val * 10 + (input[i] - '0');
        if (val > UINT32_MAX) return UINT32_MAX;
    }
    return (uint32_t)val;
}

static void state_idle(struct context *ctx) {
    if (ctx == NULL) return;
    printf("State: IDLE\n");
    ctx->current_state = STATE_PROCESSING;
    ctx->counter = 0;
}

static void state_processing(struct context *ctx) {
    if (ctx == NULL) return;
    printf("State: PROCESSING (Iteration %u)\n", ctx->counter + 1);
    
    uint64_t result = 0;
    for (uint32_t i = 0; i < 1000000; i++) {
        result += (i * i) % 1000;
    }
    
    ctx->counter++;
    if (ctx->counter >= ctx->max_iterations) {
        ctx->current_state = STATE_COMPLETED;
    } else if (ctx->counter >= ctx->error_threshold) {
        ctx->current_state = STATE_ERROR;
    }
}

static void state_completed(struct context *ctx) {
    if (ctx == NULL) return;
    printf("State: COMPLETED\n");
    printf("Total iterations: %u\n", ctx->counter);
}

static void state_error(struct context *ctx) {
    if (ctx == NULL) return;
    printf("State: ERROR\n");
    printf("Error at iteration: %u\n", ctx->counter);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <max_iterations> <error_threshold>\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    if (!validate_input(argv[1]) || !validate_input(argv[2])) {
        fprintf(stderr, "Error: Invalid input parameters\n");
        return EXIT_FAILURE;
    }
    
    struct context ctx;
    ctx.max_iterations = parse_uint32(argv[1]);
    ctx.error_threshold = parse_uint32(argv[2]);
    
    if (ctx.max_iterations == 0 || ctx.max_iterations > 1000000) {
        fprintf(stderr, "Error: Max iterations must be between 1 and 1000000\n");
        return EXIT_FAILURE;
    }
    if (ctx.error_threshold == 0 || ctx.error_threshold > ctx.max_iterations) {
        fprintf(stderr, "Error: Error threshold must be between 1 and max_iterations\n");
        return EXIT_FAILURE;
    }
    
    ctx.current_state = STATE_IDLE;
    ctx.counter = 0;
    
    while (ctx.current_state != STATE_COMPLETED && ctx.current_state != STATE_ERROR) {
        switch (ctx.current_state) {
            case STATE_IDLE:
                state_idle(&ctx);
                break;
            case STATE_PROCESSING:
                state_processing(&ctx);
                break;
            case STATE_COMPLETED:
            case STATE_ERROR:
                break;
            default:
                fprintf(stderr, "Error: Invalid state\n");
                return EXIT_FAILURE;
        }
    }
    
    if (ctx.current_state == STATE_COMPLETED) {
        state_completed(&ctx);
    } else if (ctx.current_state == STATE_ERROR) {
        state_error(&ctx);
    }
    
    return EXIT_SUCCESS;
}