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
        if (val > INT_MAX) return 0;
    }
    
    val *= sign;
    if (val < INT_MIN || val > INT_MAX) return 0;
    *result = (int)val;
    return 1;
}

void state_machine_while(struct context* ctx) {
    while (ctx->current_state != FINISHED && ctx->current_state != ERROR) {
        switch (ctx->current_state) {
            case INIT:
                ctx->data_value = 0;
                ctx->iteration_count = 0;
                ctx->current_state = READY;
                break;
            case READY:
                if (ctx->max_iterations > 0) {
                    ctx->current_state = PROCESSING;
                } else {
                    ctx->current_state = ERROR;
                }
                break;
            case PROCESSING:
                ctx->data_value += ctx->iteration_count * 2;
                ctx->iteration_count++;
                if (ctx->iteration_count >= ctx->max_iterations) {
                    ctx->current_state = FINISHED;
                }
                break;
            default:
                ctx->current_state = ERROR;
                break;
        }
    }
}

void state_machine_do_while(struct context* ctx) {
    do {
        switch (ctx->current_state) {
            case INIT:
                ctx->data_value = 0;
                ctx->iteration_count = 0;
                ctx->current_state = READY;
                break;
            case READY:
                if (ctx->max_iterations > 0 && ctx->max_iterations <= 100) {
                    ctx->current_state = PROCESSING;
                } else {
                    ctx->current_state = ERROR;
                }
                break;
            case PROCESSING:
                ctx->data_value += ctx->iteration_count * 3;
                ctx->iteration_count++;
                if (ctx->iteration_count >= ctx->max_iterations) {
                    ctx->current_state = FINISHED;
                }
                break;
            default:
                ctx->current_state = ERROR;
                break;
        }
    } while (ctx->current_state != FINISHED && ctx->current_state != ERROR);
}

void state_machine_for(struct context* ctx) {
    for (;;) {
        switch (ctx->current_state) {
            case INIT:
                ctx->data_value = 100;
                ctx->iteration_count = 0;
                ctx->current_state = READY;
                break;
            case READY:
                if (ctx->max_iterations > 0 && ctx->max_iterations <= 50) {
                    ctx->current_state = PROCESSING;
                } else {
                    ctx->current_state = ERROR;
                }
                break;
            case PROCESSING:
                ctx->data_value -= ctx->iteration_count;
                ctx->iteration_count++;
                if (ctx->iteration_count >= ctx->max_iterations) {
                    ctx->current_state = FINISHED;
                }
                break;
            default:
                ctx->current_state = ERROR;
                break;
        }
        if (ctx->current_state == FINISHED || ctx->current_state == ERROR) {
            break;
        }
    }
}

int main(void) {
    char input_buffer[32];
    int choice = 0;
    int iterations = 0;
    struct context ctx;
    
    printf("Enter loop variant (1=while, 2=do_while, 3=for): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    input_buffer[strcspn(input_buffer, "\n")] = '\0';
    
    if (!validate_input(input_buffer) || !safe_atoi(input_buffer, &choice)) {
        printf("Invalid choice\n");
        return 1;
    }
    
    if (choice < 1 || choice > 3) {
        printf("Choice must be 1-3\n");
        return 1;
    }
    
    printf("Enter iterations (1-100): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    input_buffer[strcspn(input_buffer, "\n")] = '\0';
    
    if (!validate