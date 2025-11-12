//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum state {
    INIT,
    READY,
    PROCESSING,
    FINALIZING,
    DONE,
    ERROR
};

struct context {
    enum state current_state;
    int data_value;
    int step_count;
    int max_steps;
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
                ctx->step_count = 0;
                ctx->current_state = READY;
                break;
                
            case READY:
                if (ctx->max_steps > 0) {
                    ctx->current_state = PROCESSING;
                } else {
                    ctx->current_state = ERROR;
                }
                break;
                
            case PROCESSING:
                ctx->data_value += ctx->step_count * 2;
                ctx->step_count++;
                
                if (ctx->step_count >= ctx->max_steps) {
                    ctx->current_state = FINALIZING;
                } else if (ctx->step_count > 1000) {
                    ctx->current_state = ERROR;
                }
                break;
                
            case FINALIZING:
                ctx->data_value *= 2;
                ctx->current_state = DONE;
                break;
                
            case DONE:
                running = 0;
                break;
                
            case ERROR:
                ctx->data_value = -1;
                running = 0;
                break;
        }
    }
}

int main(void) {
    char input_buffer[32];
    struct context ctx = {INIT, 0, 0, 0};
    
    printf("Enter number of processing steps (1-100): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len-1] == '\n') {
        input_buffer[len-1] = '\0';
    }
    
    if (!validate_input(input_buffer)) {
        printf("Invalid input: must be positive number 1-100\n");
        return 1;
    }
    
    int steps;
    if (!safe_atoi(input_buffer, &steps)) {
        printf("Invalid number format\n");
        return 1;
    }
    
    if (steps < 1 || steps > 100) {
        printf("Steps must be between 1 and 100\n");
        return 1;
    }
    
    ctx.max_steps = steps;
    
    int i = 0;
    do {
        if (i == 0) {
            ctx.current_state = INIT;
        }
        state_machine_loop(&ctx);
        i++;
    } while (i < 3 && ctx.current_state != DONE && ctx.current_state != ERROR);
    
    if (ctx.current_state == DONE) {
        printf("Processing completed. Result: %d\n", ctx.data_value);
    } else {
        printf("Processing failed with error\n");
    }
    
    return 0;
}