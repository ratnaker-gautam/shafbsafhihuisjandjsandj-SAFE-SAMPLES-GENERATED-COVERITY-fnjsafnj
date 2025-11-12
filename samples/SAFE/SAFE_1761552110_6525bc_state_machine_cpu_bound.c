//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum state {
    INIT,
    PROCESSING,
    VALIDATING,
    FINALIZING,
    ERROR,
    DONE
};

struct context {
    uint32_t counter;
    uint32_t iterations;
    uint32_t data[100];
    size_t data_count;
    enum state current_state;
};

static int validate_input(uint32_t iterations) {
    if (iterations == 0 || iterations > 1000000) {
        return 0;
    }
    return 1;
}

static void init_state(struct context *ctx) {
    ctx->counter = 0;
    ctx->data_count = 0;
    ctx->current_state = PROCESSING;
    memset(ctx->data, 0, sizeof(ctx->data));
}

static void processing_state(struct context *ctx) {
    if (ctx->counter >= ctx->iterations) {
        ctx->current_state = VALIDATING;
        return;
    }
    
    if (ctx->data_count < 100) {
        ctx->data[ctx->data_count] = ctx->counter * 2;
        ctx->data_count++;
    }
    
    ctx->counter++;
    
    if (ctx->counter % 10000 == 0) {
        ctx->current_state = VALIDATING;
    }
}

static void validating_state(struct context *ctx) {
    for (size_t i = 0; i < ctx->data_count; i++) {
        if (ctx->data[i] % 2 != 0) {
            ctx->current_state = ERROR;
            return;
        }
    }
    
    if (ctx->counter >= ctx->iterations) {
        ctx->current_state = FINALIZING;
    } else {
        ctx->current_state = PROCESSING;
    }
}

static void finalizing_state(struct context *ctx) {
    uint32_t sum = 0;
    for (size_t i = 0; i < ctx->data_count; i++) {
        if (sum > UINT32_MAX - ctx->data[i]) {
            ctx->current_state = ERROR;
            return;
        }
        sum += ctx->data[i];
    }
    ctx->current_state = DONE;
}

static void error_state(struct context *ctx) {
    ctx->current_state = DONE;
}

static void run_state_machine(struct context *ctx) {
    while (ctx->current_state != DONE) {
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
            case ERROR:
                error_state(ctx);
                break;
            case DONE:
                break;
        }
    }
}

int main(void) {
    struct context ctx;
    char input_buffer[32];
    uint32_t iterations;
    
    printf("Enter number of iterations (1-1000000): ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (sscanf(input_buffer, "%u", &iterations) != 1) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    if (!validate_input(iterations)) {
        fprintf(stderr, "Invalid iteration count\n");
        return 1;
    }
    
    ctx.iterations = iterations;
    ctx.current_state = INIT;
    
    run_state_machine(&ctx);
    
    if (ctx.current_state == DONE && ctx.counter == iterations) {
        printf("State machine completed successfully\n");
        printf("Processed %u iterations with %zu data points\n", 
               ctx.counter, ctx.data_count);
        return 0;
    } else {
        printf("State machine terminated with errors\n");
        return 1;
    }
}