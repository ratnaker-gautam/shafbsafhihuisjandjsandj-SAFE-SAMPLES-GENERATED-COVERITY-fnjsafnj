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

void state_machine_loop(struct context* ctx) {
    int running = 1;
    
    while (running) {
        switch (ctx->current_state) {
            case INIT:
                ctx->data_value = 0;
                ctx->iteration_count = 0;
                ctx->current_state = READY;
                break;
                
            case READY: {
                char input[32];
                printf("Enter number of iterations (1-100): ");
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    ctx->current_state = ERROR;
                    break;
                }
                
                input[strcspn(input, "\n")] = '\0';
                
                if (!validate_input(input)) {
                    ctx->current_state = ERROR;
                    break;
                }
                
                int iterations;
                if (!safe_atoi(input, &iterations)) {
                    ctx->current_state = ERROR;
                    break;
                }
                
                if (iterations < 1 || iterations > 100) {
                    ctx->current_state = ERROR;
                    break;
                }
                
                ctx->max_iterations = iterations;
                ctx->current_state = PROCESSING;
                break;
            }
            
            case PROCESSING:
                if (ctx->iteration_count >= ctx->max_iterations) {
                    ctx->current_state = FINISHED;
                    break;
                }
                
                ctx->data_value += (ctx->iteration_count + 1) * 2;
                if (ctx->data_value < 0) {
                    ctx->current_state = ERROR;
                    break;
                }
                
                ctx->iteration_count++;
                printf("Processing iteration %d, value: %d\n", 
                       ctx->iteration_count, ctx->data_value);
                
                if (ctx->iteration_count >= ctx->max_iterations) {
                    ctx->current_state = FINISHED;
                }
                break;
                
            case FINISHED:
                printf("Processing complete. Final value: %d\n", ctx->data_value);
                running = 0;
                break;
                
            case ERROR:
                printf("Error occurred during processing.\n");
                running = 0;
                break;
        }
    }
}

int main(void) {
    struct context ctx;
    ctx.current_state = INIT;
    ctx.data_value = 0;
    ctx.iteration_count = 0;
    ctx.max_iterations = 0;
    
    state_machine_loop(&ctx);
    
    return ctx.current_state == FINISHED ? 0 : 1;
}