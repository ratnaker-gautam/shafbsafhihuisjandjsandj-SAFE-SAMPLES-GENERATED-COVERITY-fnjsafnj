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
        if (input[i] < '0' || input[i] > '9') return 0;
    }
    return 1;
}

int parse_number(const char* input) {
    long value = 0;
    for (size_t i = 0; input[i] != '\0'; i++) {
        value = value * 10 + (input[i] - '0');
        if (value > INT32_MAX) return -1;
    }
    return (int)value;
}

int process_data(int value) {
    if (value < 0) return -1;
    int result = 0;
    while (value > 0) {
        int digit = value % 10;
        if (digit > 0 && result > INT32_MAX / digit) return -1;
        result = result * 10 + digit;
        value /= 10;
    }
    return result;
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
                printf("Enter a positive number (or 'quit' to exit): ");
                ctx.current_state = READ_INPUT;
                break;
                
            case READ_INPUT: {
                if (fgets(ctx.input_buffer, sizeof(ctx.input_buffer), stdin) == NULL) {
                    ctx.current_state = ERROR;
                    ctx.error_code = 1;
                    break;
                }
                
                size_t len = strlen(ctx.input_buffer);
                if (len > 0 && ctx.input_buffer[len - 1] == '\n') {
                    ctx.input_buffer[len - 1] = '\0';
                }
                
                if (strcmp(ctx.input_buffer, "quit") == 0) {
                    ctx.current_state = EXIT;
                } else {
                    ctx.current_state = VALIDATE;
                }
                break;
            }
            
            case VALIDATE:
                if (!validate_input(ctx.input_buffer)) {
                    ctx.current_state = ERROR;
                    ctx.error_code = 2;
                } else {
                    int parsed = parse_number(ctx.input_buffer);
                    if (parsed < 0) {
                        ctx.current_state = ERROR;
                        ctx.error_code = 3;
                    } else {
                        ctx.data_value = parsed;
                        ctx.current_state = PROCESS_DATA;
                    }
                }
                break;
                
            case PROCESS_DATA: {
                int result = process_data(ctx.data_value);
                if (result < 0) {
                    ctx.current_state = ERROR;
                    ctx.error_code = 4;
                } else {
                    ctx.data_value = result;
                    ctx.current_state = OUTPUT_RESULT;
                }
                break;
            }
            
            case OUTPUT_RESULT:
                printf("Reversed number: %d\n", ctx.data_value);
                ctx.current_state = INIT;
                break;
                
            case ERROR:
                switch (ctx.error_code) {
                    case 1:
                        printf("Input error occurred.\n");
                        break;
                    case 2:
                        printf("Invalid input format. Please enter only digits.\n");
                        break;
                    case 3:
                        printf("Number too large or invalid.\n");
                        break;
                    case 4:
                        printf("Processing error occurred.\n");
                        break;
                    default:
                        printf("Unknown error occurred.\n");
                        break;
                }
                ctx.current_state = INIT;
                break;
                
            case EXIT:
                break;
        }
    }
    
    printf("Goodbye!\n");
    return 0;
}