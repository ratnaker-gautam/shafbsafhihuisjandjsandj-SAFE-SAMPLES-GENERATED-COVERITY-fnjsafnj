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
    if (input == NULL) return 0;
    char *endptr;
    long long_val = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\0') return 0;
    if (long_val < 0 || long_val > UINT32_MAX) return 0;
    *value = (uint32_t)long_val;
    return 1;
}

static void state_idle(struct context *ctx) {
    if (ctx->counter >= ctx->max_iterations) {
        ctx->current_state = STATE_COMPLETED;
    } else if (ctx->counter >= ctx->error_threshold) {
        ctx->current_state = STATE_ERROR;
    } else {
        ctx->current_state = STATE_PROCESSING;
    }
}

static void state_processing(struct context *ctx) {
    uint32_t old_counter = ctx->counter;
    if (ctx->counter < UINT32_MAX - 1000) {
        for (int i = 0; i < 1000; i++) {
            ctx->counter++;
        }
    } else {
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    if (ctx->counter >= ctx->max_iterations) {
        ctx->current_state = STATE_COMPLETED;
    } else if (ctx->counter >= ctx->error_threshold) {
        ctx->current_state = STATE_ERROR;
    } else if (ctx->counter == old_counter + 1000) {
        ctx->current_state = STATE_IDLE;
    } else {
        ctx->current_state = STATE_ERROR;
    }
}

static void state_completed(struct context *ctx) {
    printf("Processing completed. Final counter: %u\n", ctx->counter);
}

static void state_error(struct context *ctx) {
    printf("Error state reached. Counter: %u\n", ctx->counter);
}

static void run_state_machine(struct context *ctx) {
    int cycle_count = 0;
    const int max_cycles = 10000;
    
    while (ctx->current_state != STATE_COMPLETED && 
           ctx->current_state != STATE_ERROR &&
           cycle_count < max_cycles) {
        
        enum state previous_state = ctx->current_state;
        
        switch (ctx->current_state) {
            case STATE_IDLE:
                state_idle(ctx);
                break;
            case STATE_PROCESSING:
                state_processing(ctx);
                break;
            case STATE_COMPLETED:
                state_completed(ctx);
                break;
            case STATE_ERROR:
                state_error(ctx);
                break;
            default:
                ctx->current_state = STATE_ERROR;
                break;
        }
        
        if (ctx->current_state == previous_state && 
            ctx->current_state != STATE_COMPLETED && 
            ctx->current_state != STATE_ERROR) {
            cycle_count++;
        } else {
            cycle_count = 0;
        }
        
        if (cycle_count >= 100) {
            ctx->current_state = STATE_ERROR;
            break;
        }
    }
    
    if (cycle_count >= max_cycles) {
        printf("State machine timeout\n");
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <max_iterations> <error_threshold>\n", argv[0]);
        return 1;
    }
    
    struct context ctx;
    ctx.current_state = STATE_IDLE;
    ctx.counter = 0;
    
    if (!validate_input(argv[1], &ctx.max_iterations)) {
        fprintf(stderr, "Invalid max_iterations value\n");
        return 1;
    }
    
    if (!validate_input(argv[2], &ctx.error_threshold)) {
        fprintf(stderr, "Invalid error_threshold value\n");
        return 1;
    }
    
    if (ctx.max_iterations == 0) {
        fprintf(stderr, "max_iterations must be greater than 0\n");
        return 1;
    }
    
    if (ctx.error_threshold >= ctx.max_iterations) {
        fprintf(stderr, "error_threshold must be less than max_iterations\n");
        return 1;
    }
    
    if (ctx.max_iterations > 1000000) {
        fprintf(stderr, "max_iterations too large\n");
        return 1;
    }
    
    printf("Starting state machine with max_iterations=%u, error_threshold=%u\n",
           ctx.max_iterations, ctx.error_threshold);
    
    run_state_machine(&ctx);
    
    return 0;
}