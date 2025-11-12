//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

enum state {
    INIT,
    PROCESSING,
    VALIDATING,
    FINALIZING,
    ERROR,
    DONE
};

struct context {
    enum state current_state;
    uint32_t counter;
    uint32_t max_iterations;
    uint32_t data[256];
    size_t data_size;
};

static int validate_input(uint32_t iterations) {
    if (iterations == 0 || iterations > 1000000) {
        return 0;
    }
    return 1;
}

static void init_state(struct context *ctx) {
    ctx->counter = 0;
    ctx->data_size = 0;
    memset(ctx->data, 0, sizeof(ctx->data));
    ctx->current_state = PROCESSING;
}

static void processing_state(struct context *ctx) {
    if (ctx->data_size >= 256) {
        ctx->current_state = ERROR;
        return;
    }
    
    uint32_t value = ctx->counter * 3 + 7;
    ctx->data[ctx->data_size] = value;
    ctx->data_size++;
    ctx->counter++;
    
    if (ctx->counter >= ctx->max_iterations) {
        ctx->current_state = VALIDATING;
    }
}

static void validating_state(struct context *ctx) {
    uint32_t sum = 0;
    for (size_t i = 0; i < ctx->data_size; i++) {
        if (sum > UINT32_MAX - ctx->data[i]) {
            ctx->current_state = ERROR;
            return;
        }
        sum += ctx->data[i];
    }
    
    if (sum % 2 == 0) {
        ctx->current_state = FINALIZING;
    } else {
        ctx->current_state = ERROR;
    }
}

static void finalizing_state(struct context *ctx) {
    uint32_t result = 0;
    for (size_t i = 0; i < ctx->data_size; i++) {
        result ^= ctx->data[i];
    }
    ctx->data[0] = result;
    ctx->current_state = DONE;
}

static void error_state(struct context *ctx) {
    memset(ctx->data, 0, sizeof(ctx->data));
    ctx->data_size = 0;
    ctx->current_state = DONE;
}

static void run_state_machine(struct context *ctx) {
    while (ctx->current_state != DONE && ctx->current_state != ERROR) {
        switch (ctx->current_state) {
            case INIT:
                init_state(ctx);
                break;
            case PROCESSING:
                processing_state(ctx);
                break;
            case VALIDATING:
                validating_state(ctx);
                break;
            case FINALIZING:
                finalizing_state(ctx);
                break;
            default:
                ctx->current_state = ERROR;
                break;
        }
    }
    
    if (ctx->current_state == ERROR) {
        error_state(ctx);
    }
}

int main(void) {
    struct context ctx;
    char input_buffer[64];
    uint32_t iterations;
    
    printf("Enter number of iterations (1-1000000): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *endptr;
    long temp = strtol(input_buffer, &endptr, 10);
    if (endptr == input_buffer || *endptr != '\n' && *endptr != '\0') {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    if (temp < 0 || temp > 1000000) {
        fprintf(stderr, "Input out of range\n");
        return 1;
    }
    
    iterations = (uint32_t)temp;
    
    if (!validate_input(iterations)) {
        fprintf(stderr, "Invalid iteration count\n");
        return 1;
    }
    
    ctx.current_state = INIT;
    ctx.max_iterations = iterations;
    
    run_state_machine(&ctx);
    
    if (ctx.current_state == DONE) {
        printf("State machine completed successfully\n");
        printf("Final result: %u\n", ctx.data[0]);
        printf("Processed %zu elements\n", ctx.data_size);
    } else {
        printf("State machine failed\n");
        return 1;
    }
    
    return 0;
}