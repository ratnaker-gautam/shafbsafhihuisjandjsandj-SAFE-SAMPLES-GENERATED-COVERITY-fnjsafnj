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

static int validate_input(const char *input, uint32_t *value) {
    if (input == NULL || value == NULL) return 0;
    
    char *endptr;
    long long_val = strtol(input, &endptr, 10);
    
    if (endptr == input || *endptr != '\0') return 0;
    if (long_val < 0 || long_val > UINT32_MAX) return 0;
    
    *value = (uint32_t)long_val;
    return 1;
}

static enum state handle_idle(struct context *ctx, uint32_t input) {
    if (ctx == NULL) return STATE_ERROR;
    
    if (input == 0) {
        return STATE_IDLE;
    }
    
    ctx->counter = 0;
    ctx->max_iterations = input;
    return STATE_PROCESSING;
}

static enum state handle_processing(struct context *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    if (ctx->counter >= ctx->max_iterations) {
        return STATE_COMPLETED;
    }
    
    uint32_t i;
    uint64_t result = 1;
    
    for (i = 1; i <= 1000; i++) {
        result = (result * i) % 1000000007ULL;
    }
    
    ctx->counter++;
    
    if (ctx->counter >= ctx->error_threshold) {
        return STATE_ERROR;
    }
    
    return STATE_PROCESSING;
}

static enum state handle_completed(struct context *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    return STATE_IDLE;
}

static enum state handle_error(struct context *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    return STATE_IDLE;
}

static enum state process_state(struct context *ctx, uint32_t input) {
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
    ctx.max_iterations = 0;
    ctx.error_threshold = 1000000;
    
    char input_buffer[64];
    uint32_t input_value;
    
    printf("State Machine Simulation\n");
    printf("Enter number of iterations (0 to exit): ");
    
    while (fgets(input_buffer, sizeof(input_buffer), stdin) != NULL) {
        size_t len = strlen(input_buffer);
        if (len > 0 && input_buffer[len - 1] == '\n') {
            input_buffer[len - 1] = '\0';
        }
        
        if (!validate_input(input_buffer, &input_value)) {
            printf("Invalid input. Enter a positive integer (0 to exit): ");
            continue;
        }
        
        if (input_value == 0) {
            break;
        }
        
        ctx.current_state = process_state(&ctx, input_value);
        
        while (ctx.current_state == STATE_PROCESSING) {
            printf("Processing iteration %u/%u\r", ctx.counter, ctx.max_iterations);
            fflush(stdout);
            ctx.current_state = process_state(&ctx, 0);
        }
        
        switch (ctx.current_state) {
            case STATE_COMPLETED:
                printf("Completed %u iterations successfully\n", ctx.max_iterations);
                break;
            case STATE_ERROR:
                printf("Error occurred during processing\n");
                break;
            case STATE_IDLE:
                printf("Ready for next task\n");
                break;
            default:
                printf("Unknown state\n");
                break;
        }
        
        printf("Enter number of iterations (0 to exit): ");
    }
    
    printf("Exiting state machine simulation\n");
    return 0;
}