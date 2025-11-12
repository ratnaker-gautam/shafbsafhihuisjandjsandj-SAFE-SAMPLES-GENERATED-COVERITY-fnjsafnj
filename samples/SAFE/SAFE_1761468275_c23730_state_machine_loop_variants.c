//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

enum state {
    INIT,
    READ_INPUT,
    PROCESS_DATA,
    VALIDATE,
    OUTPUT,
    ERROR,
    DONE
};

struct context {
    enum state current_state;
    char input_buffer[256];
    int data_value;
    int iteration_count;
    int max_iterations;
};

int validate_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > 255) return 0;
    
    for (size_t i = 0; i < len; i++) {
        if (input[i] < '0' || input[i] > '9') {
            return 0;
        }
    }
    return 1;
}

int safe_atoi(const char* str, int* result) {
    if (str == NULL || result == NULL) return 0;
    
    long value = 0;
    int sign = 1;
    size_t i = 0;
    
    if (str[0] == '-') {
        sign = -1;
        i = 1;
    }
    
    for (; str[i] != '\0'; i++) {
        if (str[i] < '0' || str[i] > '9') return 0;
        
        if (value > (INT32_MAX - (str[i] - '0')) / 10) {
            return 0;
        }
        value = value * 10 + (str[i] - '0');
    }
    
    value *= sign;
    if (value < INT32_MIN || value > INT32_MAX) return 0;
    
    *result = (int)value;
    return 1;
}

void state_machine_loop(struct context* ctx) {
    int running = 1;
    
    while (running) {
        switch (ctx->current_state) {
            case INIT:
                ctx->data_value = 0;
                ctx->iteration_count = 0;
                ctx->max_iterations = 5;
                ctx->current_state = READ_INPUT;
                break;
                
            case READ_INPUT:
                printf("Enter a number (0-9999): ");
                if (fgets(ctx->input_buffer, sizeof(ctx->input_buffer), stdin) != NULL) {
                    size_t len = strlen(ctx->input_buffer);
                    if (len > 0 && ctx->input_buffer[len - 1] == '\n') {
                        ctx->input_buffer[len - 1] = '\0';
                    }
                    ctx->current_state = VALIDATE;
                } else {
                    ctx->current_state = ERROR;
                }
                break;
                
            case VALIDATE:
                if (validate_input(ctx->input_buffer)) {
                    if (safe_atoi(ctx->input_buffer, &ctx->data_value)) {
                        if (ctx->data_value >= 0 && ctx->data_value <= 9999) {
                            ctx->current_state = PROCESS_DATA;
                        } else {
                            printf("Number out of range (0-9999)\n");
                            ctx->current_state = READ_INPUT;
                        }
                    } else {
                        printf("Invalid number format\n");
                        ctx->current_state = READ_INPUT;
                    }
                } else {
                    printf("Invalid input\n");
                    ctx->current_state = READ_INPUT;
                }
                break;
                
            case PROCESS_DATA:
                if (ctx->data_value > INT_MAX / 2) {
                    ctx->current_state = ERROR;
                } else {
                    ctx->data_value = (ctx->data_value * 2) % 10000;
                    ctx->iteration_count++;
                    ctx->current_state = OUTPUT;
                }
                break;
                
            case OUTPUT:
                printf("Iteration %d: Result = %d\n", ctx->iteration_count, ctx->data_value);
                if (ctx->iteration_count >= ctx->max_iterations) {
                    ctx->current_state = DONE;
                } else {
                    ctx->current_state = READ_INPUT;
                }
                break;
                
            case ERROR:
                printf("Input error occurred\n");
                ctx->current_state = DONE;
                break;
                
            case DONE:
                running = 0;
                break;
        }
    }
}

int main(void) {
    struct context ctx;
    ctx.current_state = INIT;
    
    printf("State Machine Demo - Number Processing\n");
    printf("Processing up to 5 numbers...\n");
    
    state_machine_loop(&ctx);
    
    printf("Processing complete. Final result: %d\n", ctx.data_value);
    
    return 0;
}