//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

enum state {
    INIT,
    READY,
    PROCESSING,
    COMPLETED,
    ERROR,
    EXIT
};

struct context {
    enum state current_state;
    int data_value;
    int step_count;
    char buffer[64];
};

int validate_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > 63) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] < '0' || input[i] > '9') return 0;
    }
    return 1;
}

enum state handle_init(struct context* ctx) {
    printf("State machine initialized\n");
    ctx->data_value = 0;
    ctx->step_count = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    return READY;
}

enum state handle_ready(struct context* ctx) {
    printf("Enter a number (or 'quit' to exit): ");
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
        return ERROR;
    }
    
    size_t len = strlen(ctx->buffer);
    if (len > 0 && ctx->buffer[len - 1] == '\n') {
        ctx->buffer[len - 1] = '\0';
    }
    
    if (strcmp(ctx->buffer, "quit") == 0) {
        return EXIT;
    }
    
    if (!validate_input(ctx->buffer)) {
        printf("Invalid input. Please enter digits only.\n");
        return READY;
    }
    
    char* endptr;
    long value = strtol(ctx->buffer, &endptr, 10);
    if (endptr == ctx->buffer || *endptr != '\0') {
        return ERROR;
    }
    if (value < 0 || value > 10000) {
        printf("Value out of range (0-10000)\n");
        return READY;
    }
    
    ctx->data_value = (int)value;
    return PROCESSING;
}

enum state handle_processing(struct context* ctx) {
    printf("Processing value: %d\n", ctx->data_value);
    
    int result = ctx->data_value;
    int steps = 0;
    
    while (result != 1 && steps < 1000) {
        if (result % 2 == 0) {
            result /= 2;
        } else {
            if (result > (INT_MAX - 1) / 3) {
                return ERROR;
            }
            result = result * 3 + 1;
        }
        steps++;
    }
    
    ctx->step_count = steps;
    printf("Completed in %d steps\n", steps);
    return COMPLETED;
}

enum state handle_completed(struct context* ctx) {
    printf("Operation completed successfully\n");
    return READY;
}

enum state handle_error(struct context* ctx) {
    printf("An error occurred\n");
    return EXIT;
}

int main(void) {
    struct context ctx;
    ctx.current_state = INIT;
    
    while (1) {
        enum state next_state;
        
        switch (ctx.current_state) {
            case INIT:
                next_state = handle_init(&ctx);
                break;
            case READY:
                next_state = handle_ready(&ctx);
                break;
            case PROCESSING:
                next_state = handle_processing(&ctx);
                break;
            case COMPLETED:
                next_state = handle_completed(&ctx);
                break;
            case ERROR:
                next_state = handle_error(&ctx);
                break;
            case EXIT:
                printf("Exiting state machine\n");
                return 0;
            default:
                next_state = ERROR;
                break;
        }
        
        ctx.current_state = next_state;
    }
    
    return 0;
}