//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    INIT,
    READ_NUM,
    READ_OP,
    CALCULATE,
    ERROR,
    EXIT
};

int main(void) {
    enum state current_state = INIT;
    double accumulator = 0.0;
    double operand = 0.0;
    char operator = '+';
    char input_buffer[256];
    int valid_input = 0;
    
    printf("Simple Calculator (Enter 'quit' to exit)\n");
    
    while (current_state != EXIT) {
        switch (current_state) {
            case INIT:
                printf("Enter initial number: ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) != NULL) {
                    if (strncmp(input_buffer, "quit", 4) == 0) {
                        current_state = EXIT;
                    } else if (sscanf(input_buffer, "%lf", &accumulator) == 1) {
                        current_state = READ_OP;
                    } else {
                        printf("Invalid number. Please try again.\n");
                    }
                }
                break;
                
            case READ_OP:
                printf("Enter operator (+, -, *, /) or 'quit': ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) != NULL) {
                    if (strncmp(input_buffer, "quit", 4) == 0) {
                        current_state = EXIT;
                    } else if (strlen(input_buffer) >= 2) {
                        char op = input_buffer[0];
                        if (op == '+' || op == '-' || op == '*' || op == '/') {
                            operator = op;
                            current_state = READ_NUM;
                        } else {
                            printf("Invalid operator. Please try again.\n");
                        }
                    }
                }
                break;
                
            case READ_NUM:
                printf("Enter next number: ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) != NULL) {
                    if (strncmp(input_buffer, "quit", 4) == 0) {
                        current_state = EXIT;
                    } else if (sscanf(input_buffer, "%lf", &operand) == 1) {
                        current_state = CALCULATE;
                    } else {
                        printf("Invalid number. Please try again.\n");
                    }
                }
                break;
                
            case CALCULATE:
                valid_input = 1;
                switch (operator) {
                    case '+':
                        accumulator += operand;
                        break;
                    case '-':
                        accumulator -= operand;
                        break;
                    case '*':
                        accumulator *= operand;
                        break;
                    case '/':
                        if (operand != 0.0) {
                            accumulator /= operand;
                        } else {
                            printf("Error: Division by zero\n");
                            valid_input = 0;
                        }
                        break;
                    default:
                        valid_input = 0;
                        break;
                }
                
                if (valid_input) {
                    printf("Result: %.2f\n", accumulator);
                    current_state = READ_OP;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case ERROR:
                printf("Calculation error occurred. Resetting...\n");
                accumulator = 0.0;
                current_state = INIT;
                break;
                
            case EXIT:
                break;
        }
    }
    
    printf("Final result: %.2f\n", accumulator);
    return 0;
}