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

void state_init(struct context* ctx) {
    ctx->current_state = READY;
    ctx->data_value = 0;
    ctx->iteration_count = 0;
    ctx->max_iterations = 5;
}

void state_ready(struct context* ctx) {
    char input[32];
    printf("Enter initial value (0-100): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        ctx->current_state = ERROR;
        return;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    if (!validate_input(input)) {
        ctx->current_state = ERROR;
        return;
    }
    
    int value;
    if (!safe_atoi(input, &value)) {
        ctx->current_state = ERROR;
        return;
    }
    
    if (value < 0 || value > 100) {
        ctx->current_state = ERROR;
        return;
    }
    
    ctx->data_value = value;
    ctx->current_state = PROCESSING;
}

void state_processing(struct context* ctx) {
    int i = 0;
    while (i < 3) {
        if (ctx->data_value > 1000) {
            ctx->current_state = ERROR;
            return;
        }
        ctx->data_value = ctx->data_value * 2 + 1;
        i++;
    }
    
    for (int j = 0; j < 2; j++) {
        if (ctx->data_value < 0) {
            ctx->current_state = ERROR;
            return;
        }
        ctx->data_value = ctx->data_value / 2;
    }
    
    do {
        ctx->data_value += ctx->iteration_count;
        ctx->iteration_count++;
    } while (ctx->iteration_count % 2 != 0 && ctx->iteration_count < 10);
    
    if (ctx->iteration_count >= ctx->max_iterations) {
        ctx->current_state = FINISHED;
    }
}

void state_finished(struct context* ctx) {
    printf("Processing complete. Final value: %d\n", ctx->data_value);
}

void state_error(struct context* ctx) {
    printf("Error state reached. Current value: %d\n", ctx->data_value);
}

int main(void) {
    struct context ctx;
    state_init(&ctx);
    
    while (1) {
        switch (ctx.current_state) {
            case INIT:
                state_init(&ctx);
                break;
            case READY:
                state_ready(&ctx);
                break;
            case PROCESSING:
                state_processing(&ctx);
                break;
            case FINISHED:
                state_finished(&ctx);
                return 0;
            case ERROR:
                state_error(&ctx);
                return 1;
        }
        
        if (ctx.iteration_count > 100) {
            printf("Iteration limit exceeded\n");
            return 1;
        }
    }
    
    return 0;
}