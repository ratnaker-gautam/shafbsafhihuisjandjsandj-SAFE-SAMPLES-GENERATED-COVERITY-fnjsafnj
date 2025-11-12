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
    uint32_t data;
    uint32_t result;
    int step_count;
    int max_steps;
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

static uint32_t process_data(uint32_t data) {
    uint32_t result = data;
    for (int i = 0; i < 1000; i++) {
        result = (result * 1103515245 + 12345) & 0x7FFFFFFF;
        result ^= (result >> 16);
        result = (result * 16807) % 2147483647;
    }
    return result;
}

static int state_init(struct context *ctx, const char *input) {
    if (ctx == NULL || input == NULL) return 0;
    ctx->data = (uint32_t)strtoul(input, NULL, 10);
    ctx->step_count = 0;
    ctx->max_steps = 10000;
    ctx->current_state = PROCESSING;
    return 1;
}

static int state_processing(struct context *ctx) {
    if (ctx == NULL) return 0;
    if (ctx->step_count >= ctx->max_steps) {
        ctx->current_state = ERROR;
        return 0;
    }
    ctx->result = process_data(ctx->data);
    ctx->step_count++;
    ctx->current_state = VALIDATING;
    return 1;
}

static int state_validating(struct context *ctx) {
    if (ctx == NULL) return 0;
    if (ctx->result == 0 || ctx->result == UINT32_MAX) {
        ctx->current_state = ERROR;
        return 0;
    }
    if (ctx->step_count >= ctx->max_steps) {
        ctx->current_state = ERROR;
        return 0;
    }
    ctx->current_state = FINALIZING;
    return 1;
}

static int state_finalizing(struct context *ctx) {
    if (ctx == NULL) return 0;
    ctx->result = (ctx->result * 6364136223846793005UL) >> 32;
    ctx->step_count++;
    ctx->current_state = DONE;
    return 1;
}

static int state_error(struct context *ctx) {
    if (ctx == NULL) return 0;
    ctx->result = 0;
    return 1;
}

static int state_done(struct context *ctx) {
    return ctx != NULL;
}

static void run_state_machine(struct context *ctx, const char *input) {
    if (ctx == NULL) return;
    
    ctx->current_state = INIT;
    
    while (ctx->current_state != DONE && ctx->current_state != ERROR) {
        switch (ctx->current_state) {
            case INIT:
                if (!state_init(ctx, input)) {
                    ctx->current_state = ERROR;
                }
                break;
            case PROCESSING:
                state_processing(ctx);
                break;
            case VALIDATING:
                state_validating(ctx);
                break;
            case FINALIZING:
                state_finalizing(ctx);
                break;
            case ERROR:
                state_error(ctx);
                break;
            case DONE:
                state_done(ctx);
                break;
        }
    }
}

int main(void) {
    char input[32];
    struct context ctx;
    
    printf("Enter a number (1-999999999): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!validate_input(input)) {
        fprintf(stderr, "Invalid input: must be numeric 1-999999999\n");
        return EXIT_FAILURE;
    }
    
    memset(&ctx, 0, sizeof(ctx));
    run_state_machine(&ctx, input);
    
    if (ctx.current_state == ERROR) {
        fprintf(stderr, "Processing error occurred\n");
        return EXIT_FAILURE;
    }
    
    printf("Result: %u\n", ctx.result);
    printf("Steps taken: %d\n", ctx.step_count);
    
    return EXIT_SUCCESS;
}