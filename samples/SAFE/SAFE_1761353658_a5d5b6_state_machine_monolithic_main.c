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
    char input[256];
    int valid_input;
    
    printf("Simple Calculator (enter 'quit' to exit)\n");
    
    while (current_state != EXIT) {
        switch (current_state) {
            case INIT:
                printf("Enter initial number: ");
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    current_state = ERROR;
                    break;
                }
                if (strncmp(input, "quit", 4) == 0) {
                    current_state = EXIT;
                    break;
                }
                valid_input = sscanf(input, "%lf", &accumulator);
                if (valid_input == 1) {
                    current_state = READ_OP;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READ_OP:
                printf("Enter operator (+, -, *, /) or 'quit': ");
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    current_state = ERROR;
                    break;
                }
                if (strncmp(input, "quit", 4) == 0) {
                    current_state = EXIT;
                    break;
                }
                valid_input = sscanf(input, " %c", &operator);
                if (valid_input == 1 && (operator == '+' || operator == '-' || 
                    operator == '*' || operator == '/')) {
                    current_state = READ_NUM;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READ_NUM:
                printf("Enter number: ");
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    current_state = ERROR;
                    break;
                }
                if (strncmp(input, "quit", 4) == 0) {
                    current_state = EXIT;
                    break;
                }
                valid_input = sscanf(input, "%lf", &operand);
                if (valid_input == 1) {
                    current_state = CALCULATE;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case CALCULATE:
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
                            current_state = ERROR;
                            break;
                        }
                        break;
                    default:
                        current_state = ERROR;
                        break;
                }
                if (current_state != ERROR) {
                    printf("Result: %.2f\n", accumulator);
                    current_state = READ_OP;
                }
                break;
                
            case ERROR:
                printf("Invalid input. Resetting calculator.\n");
                accumulator = 0.0;
                current_state = INIT;
                break;
                
            case EXIT:
                break;
        }
    }
    
    printf("Calculator terminated.\n");
    return 0;
}