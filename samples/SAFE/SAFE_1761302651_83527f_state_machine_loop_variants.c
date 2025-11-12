//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
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
    if (val < INT32_MIN || val > INT32_MAX) return 0;
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
        printf("Number out of range.\n");
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
    
    int old_value = ctx->data_value;
    
    int i = 0;
    while (i < 3) {
        if (ctx->data_value > 50) {
            ctx->data_value -= 10;
        } else {
            ctx->data_value += 5;
        }
        i++;
    }
    
    for (int j = 0; j < 2; j++) {
        if (ctx->data_value % 2 == 0) {
            ctx->data_value /= 2;
        } else {
            ctx->data_value = ctx->data_value * 3 + 1;
        }
    }
    
    do {
        if (ctx->data_value < 0) {
            ctx->data_value = 0;
        } else if (ctx->data_value > 200) {
            ctx->data_value = 200;
        }
    } while (ctx->data_value < 0 || ctx->data_value > 200);
    
    printf("Iteration %d: %d -> %d\n", ctx->iteration_count + 1, old_value, ctx->data_value);
    ctx->iteration_count++;
    
    if (ctx->data_value == 0 || ctx->data_value == 1) {
        return FINISHED;
    }
    
    return PROCESSING;
}

enum state handle_finished(struct context* ctx) {
    printf("Processing complete. Final value: %d\n", ctx->data_value);
    return FINISHED;
}

enum state handle_error(struct context* ctx) {
    printf("Error state reached. Resetting.\n");
    return INIT;
}

int main(void) {
    struct context ctx;
    ctx.current_state = INIT;
    
    while (1) {
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
                handle_finished(&ctx);
                return 0;
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