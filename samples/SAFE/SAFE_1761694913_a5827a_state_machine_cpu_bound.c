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
    char buffer[256];
};

static int validate_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len >= 255) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] < '0' || input[i] > '9') return 0;
    }
    return 1;
}

static enum state handle_idle(struct context *ctx, const char *input) {
    if (!validate_input(input)) return STATE_ERROR;
    ctx->iterations = (uint32_t)strtoul(input, NULL, 10);
    if (ctx->iterations == 0 || ctx->iterations > 1000000) return STATE_ERROR;
    ctx->counter = 0;
    return STATE_PROCESSING;
}

static enum state handle_processing(struct context *ctx) {
    if (ctx->iterations == 0) return STATE_ERROR;
    for (uint32_t i = 0; i < ctx->iterations; i++) {
        if (ctx->counter > UINT64_MAX - (uint64_t)i) {
            return STATE_ERROR;
        }
        ctx->counter += (uint64_t)i;
        for (int j = 0; j < 100; j++) {
            ctx->counter ^= (uint64_t)j;
        }
    }
    return STATE_COMPLETED;
}

static enum state handle_completed(struct context *ctx) {
    printf("Processing completed. Result: %lu\n", ctx->counter);
    return STATE_IDLE;
}

static enum state handle_error(struct context *ctx) {
    printf("Error occurred during processing.\n");
    ctx->counter = 0;
    ctx->iterations = 0;
    return STATE_IDLE;
}

static void run_state_machine(struct context *ctx) {
    while (1) {
        switch (ctx->current_state) {
            case STATE_IDLE:
                printf("Enter number of iterations (1-1000000): ");
                if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
                    ctx->current_state = STATE_ERROR;
                    break;
                }
                size_t len = strlen(ctx->buffer);
                if (len > 0 && ctx->buffer[len - 1] == '\n') {
                    ctx->buffer[len - 1] = '\0';
                }
                ctx->current_state = handle_idle(ctx, ctx->buffer);
                break;
                
            case STATE_PROCESSING:
                ctx->current_state = handle_processing(ctx);
                break;
                
            case STATE_COMPLETED:
                ctx->current_state = handle_completed(ctx);
                break;
                
            case STATE_ERROR:
                ctx->current_state = handle_error(ctx);
                break;
                
            default:
                ctx->current_state = STATE_ERROR;
                break;
        }
        
        if (ctx->current_state >= STATE_COUNT) {
            ctx->current_state = STATE_ERROR;
        }
    }
}

int main(void) {
    struct context ctx;
    ctx.current_state = STATE_IDLE;
    ctx.counter = 0;
    ctx.iterations = 0;
    memset(ctx.buffer, 0, sizeof(ctx.buffer));
    
    run_state_machine(&ctx);
    
    return 0;
}