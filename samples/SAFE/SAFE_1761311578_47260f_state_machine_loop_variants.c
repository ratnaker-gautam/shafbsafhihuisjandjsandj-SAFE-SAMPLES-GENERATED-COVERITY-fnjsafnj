//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum state {
    INIT,
    READ_INPUT,
    PROCESS_DATA,
    VALIDATE,
    OUTPUT_RESULT,
    ERROR,
    EXIT
};

struct context {
    enum state current_state;
    int data_value;
    char input_buffer[32];
    int error_code;
};

int validate_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len >= 31) return 0;
    
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
        
        value = value * 10 + (str[i] - '0');
        if (value > INT32_MAX) return 0;
    }
    
    value *= sign;
    if (value < INT32_MIN || value > INT32_MAX) return 0;
    
    *result = (int)value;
    return 1;
}

int main(void) {
    struct context ctx;
    ctx.current_state = INIT;
    ctx.data_value = 0;
    ctx.error_code = 0;
    memset(ctx.input_buffer, 0, sizeof(ctx.input_buffer));
    
    while (ctx.current_state != EXIT) {
        switch (ctx.current_state) {
            case INIT:
                printf("State Machine Calculator - Enter numbers or 'quit'\n");
                ctx.current_state = READ_INPUT;
                break;
                
            case READ_INPUT:
                printf("Enter a number: ");
                if (fgets(ctx.input_buffer, sizeof(ctx.input_buffer), stdin) != NULL) {
                    size_t len = strlen(ctx.input_buffer);
                    if (len > 0 && ctx.input_buffer[len - 1] == '\n') {
                        ctx.input_buffer[len - 1] = '\0';
                    }
                    ctx.current_state = VALIDATE;
                } else {
                    ctx.current_state = ERROR;
                    ctx.error_code = 1;
                }
                break;
                
            case VALIDATE:
                if (strcmp(ctx.input_buffer, "quit") == 0) {
                    ctx.current_state = EXIT;
                } else if (validate_input(ctx.input_buffer)) {
                    ctx.current_state = PROCESS_DATA;
                } else {
                    printf("Invalid input. Please enter a valid integer.\n");
                    ctx.current_state = READ_INPUT;
                }
                break;
                
            case PROCESS_DATA:
                if (safe_atoi(ctx.input_buffer, &ctx.data_value)) {
                    ctx.current_state = OUTPUT_RESULT;
                } else {
                    ctx.current_state = ERROR;
                    ctx.error_code = 2;
                }
                break;
                
            case OUTPUT_RESULT:
                printf("Processed value: %d\n", ctx.data_value * 2);
                ctx.current_state = READ_INPUT;
                break;
                
            case ERROR:
                printf("Error %d occurred. Resetting...\n", ctx.error_code);
                ctx.error_code = 0;
                ctx.current_state = READ_INPUT;
                break;
                
            case EXIT:
                break;
        }
    }
    
    printf("Goodbye!\n");
    return 0;
}