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
        ctx->counter += i;
        if (ctx->counter > UINT64_MAX - i) {
            return STATE_ERROR;
        }
    }
    
    return STATE_COMPLETED;
}

static enum state handle_completed(struct context *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    int written = snprintf(ctx->buffer, sizeof(ctx->buffer), 
                          "Result: %lu", (unsigned long)ctx->counter);
    if (written < 0 || (size_t)written >= sizeof(ctx->buffer)) {
        return STATE_ERROR;
    }
    
    return STATE_IDLE;
}

static enum state handle_error(struct context *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    strncpy(ctx->buffer, "Error: Invalid input or computation overflow", 
            sizeof(ctx->buffer) - 1);
    ctx->buffer[sizeof(ctx->buffer) - 1] = '\0';
    
    return STATE_IDLE;
}

static enum state process_state(struct context *ctx, const char *input) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (ctx->current_state) {
        case STATE_IDLE:
            return handle_idle(ctx, input);
        case STATE_PROCESSING:
            return handle_processing(ctx);
        case STATE_COMPLETED:
            return handle_completed(ctx);
        case STATE_ERROR:
            return handle_error(ctx);
        default:
            return STATE_ERROR;
    }
}

int main(void) {
    struct context ctx;
    ctx.current_state = STATE_IDLE;
    ctx.counter = 0;
    ctx.iterations = 0;
    memset(ctx.buffer, 0, sizeof(ctx.buffer));
    
    char input[256];
    
    printf("State Machine CPU Benchmark\n");
    printf("Enter number of iterations (1-1000000) or 'quit' to exit:\n");
    
    while (1) {
        if (ctx.current_state == STATE_IDLE) {
            printf("> ");
            fflush(stdout);
            
            if (fgets(input, sizeof(input), stdin) == NULL) {
                break;
            }
            
            input[strcspn(input, "\n")] = '\0';
            
            if (strcmp(input, "quit") == 0) {
                break;
            }
        }
        
        enum state next_state = process_state(&ctx, input);
        ctx.current_state = next_state;
        
        if (ctx.current_state == STATE_COMPLETED || 
            ctx.current_state == STATE_ERROR) {
            if (strlen(ctx.buffer) > 0) {
                printf("%s\n", ctx.buffer);
            }
        }
        
        if (ctx.current_state == STATE_PROCESSING) {
            printf("Processing...\n");
        }
    }
    
    printf("Exiting state machine.\n");
    return 0;
}