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
                printf("Enter initial value (0-100): ");
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    ctx->current_state = ERROR;
                    break;
                }
                
                size_t len = strlen(input);
                if (len > 0 && input[len-1] == '\n') {
                    input[len-1] = '\0';
                }
                
                if (!validate_input(input)) {
                    ctx->current_state = ERROR;
                    break;
                }
                
                int value;
                if (!safe_atoi(input, &value)) {
                    ctx->current_state = ERROR;
                    break;
                }
                
                if (value < 0 || value > 100) {
                    ctx->current_state = ERROR;
                    break;
                }
                
                ctx->data_value = value;
                ctx->current_state = PROCESSING;
                break;
            }
            
            case PROCESSING: {
                if (ctx->iteration_count >= ctx->max_iterations) {
                    ctx->current_state = FINISHED;
                    break;
                }
                
                int i = 0;
                do {
                    if (ctx->data_value > 1000) {
                        ctx->current_state = ERROR;
                        break;
                    }
                    ctx->data_value += (i + 1) * 2;
                    i++;
                } while (i < 3 && ctx->current_state == PROCESSING);
                
                ctx->iteration_count++;
                
                if (ctx->iteration_count % 2 == 0) {
                    for (int j = 0; j < 2; j++) {
                        if (ctx->data_value > 0) {
                            ctx->data_value -= 5;
                        }
                    }
                }
                
                if (ctx->data_value < 0) {
                    ctx->current_state = ERROR;
                }
                break;
            }
            
            case FINISHED:
                printf("Final result: %d\n", ctx->data_value);
                running = 0;
                break;
                
            case ERROR:
                printf("Error occurred in processing\n");
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
    ctx.max_iterations = 5;
    
    state_machine_loop(&ctx);
    
    return 0;
}