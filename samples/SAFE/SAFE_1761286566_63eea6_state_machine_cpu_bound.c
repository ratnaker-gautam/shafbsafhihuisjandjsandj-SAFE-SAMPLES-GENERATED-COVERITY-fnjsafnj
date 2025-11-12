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
    uint32_t data_value;
    uint32_t iteration_count;
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

static uint32_t parse_input(const char *input) {
    char *endptr;
    unsigned long value = strtoul(input, &endptr, 10);
    if (endptr == input || *endptr != '\0') return 0;
    if (value > UINT32_MAX) return 0;
    return (uint32_t)value;
}

static enum state handle_idle(struct context *ctx, uint32_t input) {
    if (input == 0) return STATE_ERROR;
    ctx->data_value = input;
    ctx->iteration_count = 0;
    ctx->max_iterations = (input % 1000) + 1;
    return STATE_PROCESSING;
}

static enum state handle_processing(struct context *ctx) {
    if (ctx->iteration_count >= ctx->max_iterations) {
        return STATE_COMPLETED;
    }
    
    uint32_t old_value = ctx->data_value;
    for (uint32_t i = 0; i < 1000; i++) {
        ctx->data_value = (ctx->data_value * 1103515245 + 12345) & 0x7FFFFFFF;
    }
    
    ctx->iteration_count++;
    
    if (ctx->data_value == old_value) {
        return STATE_ERROR;
    }
    
    return STATE_PROCESSING;
}

static enum state handle_completed(struct context *ctx) {
    return STATE_IDLE;
}

static enum state handle_error(struct context *ctx) {
    ctx->data_value = 0;
    ctx->iteration_count = 0;
    return STATE_IDLE;
}

static void run_state_machine(struct context *ctx, uint32_t input) {
    enum state next_state = ctx->current_state;
    
    switch (ctx->current_state) {
        case STATE_IDLE:
            next_state = handle_idle(ctx, input);
            break;
        case STATE_PROCESSING:
            next_state = handle_processing(ctx);
            break;
        case STATE_COMPLETED:
            next_state = handle_completed(ctx);
            break;
        case STATE_ERROR:
            next_state = handle_error(ctx);
            break;
        default:
            next_state = STATE_ERROR;
            break;
    }
    
    ctx->current_state = next_state;
}

int main(void) {
    struct context ctx;
    ctx.current_state = STATE_IDLE;
    ctx.data_value = 0;
    ctx.iteration_count = 0;
    ctx.max_iterations = 0;
    
    char input_buffer[32];
    
    printf("State Machine Processor\n");
    printf("Enter numbers to process (0 to exit):\n");
    
    while (1) {
        printf("State: ");
        switch (ctx.current_state) {
            case STATE_IDLE: printf("IDLE"); break;
            case STATE_PROCESSING: printf("PROCESSING"); break;
            case STATE_COMPLETED: printf("COMPLETED"); break;
            case STATE_ERROR: printf("ERROR"); break;
            default: printf("UNKNOWN"); break;
        }
        printf(" | Value: %u | Iteration: %u/%u\n", 
               ctx.data_value, ctx.iteration_count, ctx.max_iterations);
        
        printf("Input: ");
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input_buffer);
        if (len > 0 && input_buffer[len - 1] == '\n') {
            input_buffer[len - 1] = '\0';
        }
        
        if (!validate_input(input_buffer)) {
            printf("Invalid input. Please enter a positive number.\n");
            continue;
        }
        
        uint32_t input_value = parse_input(input_buffer);
        if (input_value == 0) {
            break;
        }
        
        run_state_machine(&ctx, input_value);
        
        while (ctx.current_state == STATE_PROCESSING) {
            run_state_machine(&ctx, 0);
        }
    }
    
    printf("Exiting state machine processor.\n");
    return 0;
}