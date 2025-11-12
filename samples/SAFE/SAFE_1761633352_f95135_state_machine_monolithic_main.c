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
    int valid_input = 0;
    
    printf("Simple Calculator (enter 'quit' to exit)\n");
    printf("Current value: 0.0\n");
    
    while (current_state != EXIT) {
        valid_input = 0;
        
        switch (current_state) {
            case INIT:
                printf("Enter number: ");
                break;
                
            case READ_NUM:
                printf("Enter number: ");
                break;
                
            case READ_OP:
                printf("Enter operator (+, -, *, /): ");
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
                    printf("Current value: %.2f\n", accumulator);
                    current_state = READ_OP;
                }
                break;
                
            case ERROR:
                printf("Invalid input. Resetting calculator.\n");
                accumulator = 0.0;
                operator = '+';
                printf("Current value: 0.0\n");
                current_state = INIT;
                break;
                
            case EXIT:
                break;
        }
        
        if (current_state == EXIT) {
            break;
        }
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            current_state = EXIT;
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
            len--;
        }
        
        if (len == 0) {
            continue;
        }
        
        for (size_t i = 0; i < len; i++) {
            input[i] = tolower(input[i]);
        }
        
        if (strcmp(input, "quit") == 0) {
            current_state = EXIT;
            continue;
        }
        
        if (current_state == INIT || current_state == READ_NUM) {
            char *endptr;
            double num = strtod(input, &endptr);
            if (endptr != input && *endptr == '\0') {
                operand = num;
                valid_input = 1;
                if (current_state == INIT) {
                    accumulator = operand;
                    current_state = READ_OP;
                } else {
                    current_state = CALCULATE;
                }
            } else {
                current_state = ERROR;
            }
        } else if (current_state == READ_OP) {
            if (strlen(input) == 1) {
                char op = input[0];
                if (op == '+' || op == '-' || op == '*' || op == '/') {
                    operator = op;
                    valid_input = 1;
                    current_state = READ_NUM;
                } else {
                    current_state = ERROR;
                }
            } else {
                current_state = ERROR;
            }
        }
        
        if (!valid_input && current_state != ERROR && current_state != EXIT) {
            current_state = ERROR;
        }
    }
    
    printf("Calculator exited.\n");
    return 0;
}