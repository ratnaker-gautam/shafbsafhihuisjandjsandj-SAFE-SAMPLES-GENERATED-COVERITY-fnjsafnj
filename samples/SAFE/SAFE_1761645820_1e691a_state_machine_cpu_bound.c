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
    uint64_t counter;
    uint32_t iterations;
    char buffer[64];
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
    uint64_t result = 0;
    for (size_t i = 0; input[i] != '\0'; i++) {
        if (result > UINT32_MAX / 10) return UINT32_MAX;
        result = result * 10 + (input[i] - '0');
    }
    return (result > UINT32_MAX) ? UINT32_MAX : (uint32_t)result;
}

static void state_idle(struct context *ctx) {
    ctx->counter = 0;
    snprintf(ctx->buffer, sizeof(ctx->buffer), "IDLE");
    ctx->current_state = STATE_PROCESSING;
}

static void state_processing(struct context *ctx) {
    if (ctx->iterations == 0) {
        ctx->current_state = STATE_COMPLETED;
        return;
    }
    
    uint64_t sum = 0;
    for (uint32_t i = 0; i < ctx->iterations; i++) {
        for (uint32_t j = 0; j < 1000; j++) {
            sum += (i * j) % 100;
        }
    }
    
    ctx->counter += sum;
    snprintf(ctx->buffer, sizeof(ctx->buffer), "PROCESSED: %lu", (unsigned long)ctx->counter);
    
    if (ctx->counter > 1000000) {
        ctx->current_state = STATE_COMPLETED;
    } else if (ctx->iterations > 100000) {
        ctx->current_state = STATE_ERROR;
    }
}

static void state_completed(struct context *ctx) {
    snprintf(ctx->buffer, sizeof(ctx->buffer), "COMPLETED: %lu", (unsigned long)ctx->counter);
}

static void state_error(struct context *ctx) {
    snprintf(ctx->buffer, sizeof(ctx->buffer), "ERROR: Counter overflow risk");
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <iterations>\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    if (!validate_input(argv[1])) {
        fprintf(stderr, "Error: Invalid input. Provide a positive integer (1-10 digits)\n");
        return EXIT_FAILURE;
    }
    
    struct context ctx;
    ctx.current_state = STATE_IDLE;
    ctx.counter = 0;
    ctx.iterations = parse_uint32(argv[1]);
    
    if (ctx.iterations == 0 || ctx.iterations > 100000) {
        fprintf(stderr, "Error: Iterations must be between 1 and 100000\n");
        return EXIT_FAILURE;
    }
    
    while (ctx.current_state != STATE_COMPLETED && ctx.current_state != STATE_ERROR) {
        switch (ctx.current_state) {
            case STATE_IDLE:
                state_idle(&ctx);
                break;
            case STATE_PROCESSING:
                state_processing(&ctx);
                break;
            case STATE_COMPLETED:
                state_completed(&ctx);
                break;
            case STATE_ERROR:
                state_error(&ctx);
                break;
            default:
                ctx.current_state = STATE_ERROR;
                break;
        }
        
        if (ctx.current_state != STATE_COMPLETED && ctx.current_state != STATE_ERROR) {
            printf("State: %s\n", ctx.buffer);
        }
    }
    
    printf("Final: %s\n", ctx.buffer);
    
    return (ctx.current_state == STATE_COMPLETED) ? EXIT_SUCCESS : EXIT_FAILURE;
}