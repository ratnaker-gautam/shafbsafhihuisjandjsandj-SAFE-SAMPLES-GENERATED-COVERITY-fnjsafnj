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
    if (ctx == NULL) return ERROR;
    ctx->data_value = 0;
    ctx->iteration_count = 0;
    ctx->max_iterations = 5;
    return READY;
}

enum state handle_ready(struct context* ctx) {
    if (ctx == NULL) return ERROR;
    char input[32];
    printf("Enter a number (0-100): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) return ERROR;
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') input[len-1] = '\0';
    
    if (!validate_input(input)) return ERROR;
    
    int value;
    if (!safe_atoi(input, &value)) return ERROR;
    if (value < 0 || value > 100) return ERROR;
    
    ctx->data_value = value;
    return PROCESSING;
}

enum state handle_processing(struct context* ctx) {
    if (ctx == NULL) return ERROR;
    
    int result = 0;
    int i = 0;
    while (i < ctx->data_value) {
        if (i > 1000) break;
        result += i * 2;
        if (result < 0) return ERROR;
        i++;
    }
    
    int j;
    for (j = 0; j < 3; j++) {
        result -= j;
        if (result < 0) result = 0;
    }
    
    do {
        result++;
        if (result > 10000) result = 10000;
    } while (result < 10);
    
    printf("Processing result: %d\n", result);
    ctx->iteration_count++;
    
    if (ctx->iteration_count >= ctx->max_iterations) return FINISHED;
    return READY;
}

enum state handle_finished(struct context* ctx) {
    if (ctx == NULL) return ERROR;
    printf("Completed %d iterations. Final value: %d\n", 
           ctx->iteration_count, ctx->data_value);
    return FINISHED;
}

enum state handle_error(struct context* ctx) {
    if (ctx == NULL) return ERROR;
    printf("Error occurred in state machine\n");
    return ERROR;
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
            case FINISHED:
                next_state = handle_finished(&ctx);
                break;
            case ERROR:
                next_state = handle_error(&ctx);
                break;
            default:
                next_state = ERROR;
                break;
        }
        
        if (next_state == FINISHED) break;
        if (next_state == ERROR) {
            handle_error(&ctx);
            break;
        }
        
        ctx.current_state = next_state;
    }
    
    return 0;
}