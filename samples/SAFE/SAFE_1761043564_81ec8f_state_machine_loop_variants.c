//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum state {
    INIT,
    READY,
    PROCESSING,
    FINISHED,
    ERROR
};

struct context {
    enum state current_state;
    int data_value;
    int iteration_count;
    int max_iterations;
};

int validate_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > 10) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] < '0' || input[i] > '9') return 0;
    }
    return 1;
}

int safe_atoi(const char* str, int* result) {
    if (str == NULL || result == NULL) return 0;
    long val = 0;
    int sign = 1;
    size_t i = 0;
    
    if (str[0] == '-') {
        sign = -1;
        i = 1;
    }
    
    for (; str[i] != '\0'; i++) {
        if (str[i] < '0' || str[i] > '9') return 0;
        val = val * 10 + (str[i] - '0');
        if (val > INT32_MAX) return 0;
    }
    
    val *= sign;
    if (val > INT32_MAX || val < INT32_MIN) return 0;
    *result = (int)val;
    return 1;
}

enum state handle_init(struct context* ctx) {
    ctx->data_value = 0;
    ctx->iteration_count = 0;
    ctx->max_iterations = 5;
    return READY;
}

enum state handle_ready(struct context* ctx) {
    char input[32];
    printf("Enter initial value (0-100): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return ERROR;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (!validate_input(input)) {
        printf("Invalid input. Must be digits only.\n");
        return ERROR;
    }
    
    int value;
    if (!safe_atoi(input, &value)) {
        printf("Number too large or invalid.\n");
        return ERROR;
    }
    
    if (value < 0 || value > 100) {
        printf("Value must be between 0 and 100.\n");
        return ERROR;
    }
    
    ctx->data_value = value;
    return PROCESSING;
}

enum state handle_processing(struct context* ctx) {
    if (ctx->iteration_count >= ctx->max_iterations) {
        return FINISHED;
    }
    
    int i = 0;
    int sum = 0;
    while (i <= ctx->iteration_count) {
        sum += ctx->data_value + i;
        if (sum > 1000) {
            sum = 1000;
            break;
        }
        i++;
    }
    
    int j;
    int product = 1;
    for (j = 1; j <= (ctx->iteration_count + 1); j++) {
        if (j > 10) break;
        product *= j;
        if (product > 10000) {
            product = 10000;
            break;
        }
    }
    
    ctx->data_value = (sum + product) % 100;
    ctx->iteration_count++;
    
    printf("Iteration %d: value = %d\n", ctx->iteration_count, ctx->data_value);
    
    if (ctx->iteration_count >= ctx->max_iterations) {
        return FINISHED;
    }
    
    return PROCESSING;
}

enum state handle_finished(struct context* ctx) {
    printf("Processing complete. Final value: %d\n", ctx->data_value);
    return FINISHED;
}

enum state handle_error(struct context* ctx) {
    printf("Error state encountered. Resetting.\n");
    ctx->data_value = 0;
    ctx->iteration_count = 0;
    return INIT;
}

int main(void) {
    struct context ctx;
    ctx.current_state = INIT;
    ctx.data_value = 0;
    ctx.iteration_count = 0;
    ctx.max_iterations = 5;
    
    int running = 1;
    while (running) {
        switch (ctx.current_state) {
            case INIT:
                ctx.current_state = handle_init(&ctx);
                break;
            case READY:
                ctx.current_state = handle_ready(&ctx);
                break;
            case PROCESSING:
                ctx.current_state = handle_processing(&ctx);
                break;
            case FINISHED:
                ctx.current_state = handle_finished(&ctx);
                running = 0;
                break;
            case ERROR:
                ctx.current_state = handle_error(&ctx);
                break;
            default:
                ctx.current_state = ERROR;
                break;
        }
    }
    
    return 0;
}