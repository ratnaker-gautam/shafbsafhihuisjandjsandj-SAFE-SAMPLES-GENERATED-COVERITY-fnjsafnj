//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

enum state {
    STATE_IDLE,
    STATE_PROCESSING,
    STATE_COMPLETE,
    STATE_ERROR,
    STATE_COUNT
};

struct context {
    enum state current_state;
    uint32_t data;
    uint32_t iterations;
    uint32_t processed_count;
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
        result = result * 10 + (input[i] - '0');
        if (result > UINT32_MAX) return UINT32_MAX;
    }
    return (uint32_t)result;
}

static void state_idle(struct context *ctx) {
    if (ctx->iterations == 0) {
        ctx->current_state = STATE_COMPLETE;
    } else {
        ctx->current_state = STATE_PROCESSING;
        ctx->data = 1;
        ctx->processed_count = 0;
    }
}

static void state_processing(struct context *ctx) {
    if (ctx->processed_count >= ctx->iterations) {
        ctx->current_state = STATE_COMPLETE;
        return;
    }
    
    uint32_t old_data = ctx->data;
    for (int i = 0; i < 1000; i++) {
        ctx->data = (ctx->data * 1103515245 + 12345) & 0x7FFFFFFF;
    }
    
    if (ctx->data == old_data) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    ctx->processed_count++;
    if (ctx->processed_count >= ctx->iterations) {
        ctx->current_state = STATE_COMPLETE;
    }
}

static void state_complete(struct context *ctx) {
    printf("Processing complete. Final data: %u\n", ctx->data);
}

static void state_error(struct context *ctx) {
    printf("Error: Processing stalled\n");
}

static void run_state_machine(struct context *ctx) {
    while (ctx->current_state != STATE_COMPLETE && ctx->current_state != STATE_ERROR) {
        switch (ctx->current_state) {
            case STATE_IDLE:
                state_idle(ctx);
                break;
            case STATE_PROCESSING:
                state_processing(ctx);
                break;
            case STATE_COMPLETE:
                state_complete(ctx);
                break;
            case STATE_ERROR:
                state_error(ctx);
                break;
            default:
                ctx->current_state = STATE_ERROR;
                break;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <iterations>\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    if (!validate_input(argv[1])) {
        fprintf(stderr, "Error: Invalid input. Please provide a positive integer.\n");
        return EXIT_FAILURE;
    }
    
    uint32_t iterations = parse_uint32(argv[1]);
    if (iterations == 0 || iterations > 1000000) {
        fprintf(stderr, "Error: Iterations must be between 1 and 1000000\n");
        return EXIT_FAILURE;
    }
    
    struct context ctx = {
        .current_state = STATE_IDLE,
        .data = 0,
        .iterations = iterations,
        .processed_count = 0
    };
    
    run_state_machine(&ctx);
    
    return (ctx.current_state == STATE_COMPLETE) ? EXIT_SUCCESS : EXIT_FAILURE;
}