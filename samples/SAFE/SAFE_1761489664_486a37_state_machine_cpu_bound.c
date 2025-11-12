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
    uint32_t max_iterations;
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
    unsigned long val = strtoul(input, NULL, 10);
    if (val == ULONG_MAX || val > UINT32_MAX) return 0;
    return (uint32_t)val;
}

static enum state handle_idle(struct context *ctx, uint32_t input) {
    if (input == 0) return STATE_ERROR;
    ctx->max_iterations = input;
    ctx->counter = 0;
    ctx->iterations = 0;
    return STATE_PROCESSING;
}

static enum state handle_processing(struct context *ctx) {
    if (ctx->iterations >= ctx->max_iterations) {
        return STATE_COMPLETED;
    }
    for (uint32_t i = 0; i < 1000; i++) {
        ctx->counter += (i * i) % 7919;
    }
    ctx->iterations++;
    if (ctx->iterations < ctx->max_iterations) {
        return STATE_PROCESSING;
    }
    return STATE_COMPLETED;
}

static enum state handle_completed(struct context *ctx) {
    printf("Processing completed. Counter: %lu\n", (unsigned long)ctx->counter);
    return STATE_IDLE;
}

static enum state handle_error(struct context *ctx) {
    printf("Invalid input detected.\n");
    return STATE_IDLE;
}

static void run_state_machine(void) {
    struct context ctx = {STATE_IDLE, 0, 0, 0};
    char input_buffer[32];
    
    while (1) {
        switch (ctx.current_state) {
            case STATE_IDLE:
                printf("Enter number of iterations (1-1000000) or 'quit': ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) break;
                input_buffer[strcspn(input_buffer, "\n")] = '\0';
                if (strcmp(input_buffer, "quit") == 0) return;
                if (!validate_input(input_buffer)) {
                    ctx.current_state = STATE_ERROR;
                } else {
                    uint32_t iterations = parse_uint32(input_buffer);
                    ctx.current_state = handle_idle(&ctx, iterations);
                }
                break;
                
            case STATE_PROCESSING:
                ctx.current_state = handle_processing(&ctx);
                break;
                
            case STATE_COMPLETED:
                ctx.current_state = handle_completed(&ctx);
                break;
                
            case STATE_ERROR:
                ctx.current_state = handle_error(&ctx);
                break;
                
            default:
                ctx.current_state = STATE_IDLE;
                break;
        }
    }
}

int main(void) {
    run_state_machine();
    return 0;
}