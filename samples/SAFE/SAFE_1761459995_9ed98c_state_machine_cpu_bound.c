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
    if (len == 0 || len >= 100) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] < '0' || input[i] > '9') return 0;
    }
    return 1;
}

static enum state handle_idle(struct context *ctx, const char *input) {
    if (ctx == NULL || input == NULL) return STATE_ERROR;
    
    if (!validate_input(input)) return STATE_ERROR;
    
    unsigned long val = strtoul(input, NULL, 10);
    if (val == 0 || val > 1000000) return STATE_ERROR;
    
    ctx->iterations = (uint32_t)val;
    ctx->counter = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    
    return STATE_PROCESSING;
}

static enum state handle_processing(struct context *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    if (ctx->iterations == 0) return STATE_COMPLETED;
    
    for (uint32_t i = 0; i < ctx->iterations; i++) {
        for (int j = 0; j < 1000; j++) {
            ctx->counter += (i * j) % 100;
        }
    }
    
    snprintf(ctx->buffer, sizeof(ctx->buffer), "Processed %u iterations", ctx->iterations);
    
    return STATE_COMPLETED;
}

static enum state handle_completed(struct context *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    printf("Result: %s\n", ctx->buffer);
    printf("Final counter value: %lu\n", (unsigned long)ctx->counter);
    
    return STATE_IDLE;
}

static enum state handle_error(struct context *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    printf("Error: Invalid input or state transition\n");
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->counter = 0;
    ctx->iterations = 0;
    
    return STATE_IDLE;
}

static enum state (*state_handlers[STATE_COUNT])(struct context *, const char *) = {
    handle_idle,
    handle_processing,
    handle_completed,
    handle_error
};

int main(void) {
    struct context ctx;
    ctx.current_state = STATE_IDLE;
    ctx.counter = 0;
    ctx.iterations = 0;
    memset(ctx.buffer, 0, sizeof(ctx.buffer));
    
    char input[256];
    
    printf("State Machine Processor\n");
    printf("Enter number of iterations (1-1000000) or 'quit' to exit:\n");
    
    while (1) {
        if (ctx.current_state == STATE_IDLE) {
            printf("> ");
            fflush(stdout);
            
            if (fgets(input, sizeof(input), stdin) == NULL) {
                break;
            }
            
            input[strcspn(input, "\n")] = 0;
            
            if (strcmp(input, "quit") == 0) {
                break;
            }
        }
        
        enum state next_state;
        if (ctx.current_state == STATE_IDLE) {
            next_state = state_handlers[ctx.current_state](&ctx, input);
        } else if (ctx.current_state == STATE_PROCESSING) {
            next_state = state_handlers[ctx.current_state](&ctx, NULL);
        } else {
            next_state = state_handlers[ctx.current_state](&ctx, NULL);
        }
        
        if (next_state < STATE_IDLE || next_state >= STATE_COUNT) {
            ctx.current_state = STATE_ERROR;
        } else {
            ctx.current_state = next_state;
        }
    }
    
    printf("Exiting state machine processor\n");
    return 0;
}