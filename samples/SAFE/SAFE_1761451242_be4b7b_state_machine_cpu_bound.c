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
    uint32_t counter;
    uint32_t max_iterations;
    char buffer[256];
    size_t buffer_pos;
};

static void state_idle_enter(struct context *ctx) {
    ctx->counter = 0;
    ctx->buffer_pos = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

static enum state state_idle_run(struct context *ctx) {
    if (ctx->buffer_pos >= sizeof(ctx->buffer) - 1) {
        return STATE_ERROR;
    }
    
    char input;
    if (scanf("%c", &input) != 1) {
        return STATE_ERROR;
    }
    
    if (input == 's') {
        return STATE_PROCESSING;
    } else if (input == 'q') {
        return STATE_COMPLETE;
    } else {
        ctx->buffer[ctx->buffer_pos++] = input;
        return STATE_IDLE;
    }
}

static enum state state_processing_run(struct context *ctx) {
    if (ctx->counter >= ctx->max_iterations) {
        return STATE_COMPLETE;
    }
    
    uint32_t i;
    uint64_t result = 1;
    for (i = 1; i <= 1000; i++) {
        if (result > UINT64_MAX / i) {
            return STATE_ERROR;
        }
        result *= i;
        result %= 1000000007;
    }
    
    ctx->counter++;
    
    if (ctx->counter % 100 == 0) {
        return STATE_COMPLETE;
    }
    
    return STATE_PROCESSING;
}

static void state_complete_enter(struct context *ctx) {
    printf("Processing complete. Iterations: %u\n", ctx->counter);
    printf("Buffer contents: %s\n", ctx->buffer);
}

static void state_error_enter(struct context *ctx) {
    printf("Error state reached. Counter: %u\n", ctx->counter);
}

int main(void) {
    struct context ctx;
    ctx.current_state = STATE_IDLE;
    ctx.max_iterations = 1000;
    
    int valid_input = 0;
    char input_buffer[32];
    
    printf("Enter max iterations (1-10000): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    
    char *endptr;
    long value = strtol(input_buffer, &endptr, 10);
    if (endptr == input_buffer || *endptr != '\n') {
        return EXIT_FAILURE;
    }
    if (value < 1 || value > 10000) {
        return EXIT_FAILURE;
    }
    ctx.max_iterations = (uint32_t)value;
    
    printf("State machine started. Press 's' to start processing, 'q' to quit\n");
    
    while (ctx.current_state != STATE_COMPLETE && ctx.current_state != STATE_ERROR) {
        enum state next_state = ctx.current_state;
        
        switch (ctx.current_state) {
            case STATE_IDLE:
                if (ctx.current_state == STATE_IDLE) {
                    state_idle_enter(&ctx);
                }
                next_state = state_idle_run(&ctx);
                break;
                
            case STATE_PROCESSING:
                next_state = state_processing_run(&ctx);
                break;
                
            case STATE_COMPLETE:
                state_complete_enter(&ctx);
                break;
                
            case STATE_ERROR:
                state_error_enter(&ctx);
                break;
                
            default:
                next_state = STATE_ERROR;
                break;
        }
        
        if (next_state >= STATE_COUNT) {
            ctx.current_state = STATE_ERROR;
        } else {
            ctx.current_state = next_state;
        }
    }
    
    if (ctx.current_state == STATE_COMPLETE) {
        state_complete_enter(&ctx);
    } else if (ctx.current_state == STATE_ERROR) {
        state_error_enter(&ctx);
    }
    
    return EXIT_SUCCESS;
}