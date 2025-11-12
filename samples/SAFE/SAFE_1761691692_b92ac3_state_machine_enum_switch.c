//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    INIT,
    READ_NUM,
    READ_OP,
    CALCULATE,
    DISPLAY,
    ERROR,
    EXIT
};

enum operation {
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    NONE
};

int main(void) {
    enum state current_state = INIT;
    enum operation current_op = NONE;
    double operand1 = 0.0;
    double operand2 = 0.0;
    double result = 0.0;
    char input_buffer[256];
    int valid_input;
    
    printf("Simple Calculator (type 'quit' to exit)\n");
    
    while (current_state != EXIT) {
        switch (current_state) {
            case INIT:
                printf("Enter first number: ");
                current_state = READ_NUM;
                break;
                
            case READ_NUM:
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    current_state = EXIT;
                    break;
                }
                
                input_buffer[strcspn(input_buffer, "\n")] = '\0';
                
                if (strcmp(input_buffer, "quit") == 0) {
                    current_state = EXIT;
                    break;
                }
                
                valid_input = 0;
                if (sscanf(input_buffer, "%lf", &operand1) == 1) {
                    valid_input = 1;
                }
                
                if (!valid_input) {
                    printf("Invalid number. Please try again.\n");
                    current_state = READ_NUM;
                } else {
                    current_state = READ_OP;
                }
                break;
                
            case READ_OP:
                printf("Enter operation (+, -, *, /): ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    current_state = EXIT;
                    break;
                }
                
                input_buffer[strcspn(input_buffer, "\n")] = '\0';
                
                if (strcmp(input_buffer, "quit") == 0) {
                    current_state = EXIT;
                    break;
                }
                
                if (strlen(input_buffer) != 1) {
                    printf("Invalid operation. Please use +, -, *, or /.\n");
                    current_state = READ_OP;
                    break;
                }
                
                switch (input_buffer[0]) {
                    case '+': current_op = ADD; break;
                    case '-': current_op = SUBTRACT; break;
                    case '*': current_op = MULTIPLY; break;
                    case '/': current_op = DIVIDE; break;
                    default:
                        printf("Invalid operation. Please use +, -, *, or /.\n");
                        current_state = READ_OP;
                        break;
                }
                
                if (current_op != NONE) {
                    current_state = READ_NUM;
                    printf("Enter second number: ");
                }
                break;
                
            case CALCULATE:
                switch (current_op) {
                    case ADD:
                        result = operand1 + operand2;
                        break;
                    case SUBTRACT:
                        result = operand1 - operand2;
                        break;
                    case MULTIPLY:
                        result = operand1 * operand2;
                        break;
                    case DIVIDE:
                        if (operand2 == 0.0) {
                            printf("Error: Division by zero.\n");
                            current_state = ERROR;
                            break;
                        }
                        result = operand1 / operand2;
                        break;
                    default:
                        current_state = ERROR;
                        break;
                }
                
                if (current_state != ERROR) {
                    current_state = DISPLAY;
                }
                break;
                
            case DISPLAY:
                printf("Result: %.2f\n", result);
                printf("\n");
                current_state = INIT;
                current_op = NONE;
                break;
                
            case ERROR:
                printf("Calculation error occurred.\n");
                printf("\n");
                current_state = INIT;
                current_op = NONE;
                break;
                
            case EXIT:
                break;
        }
        
        if (current_state == READ_NUM && current_op != NONE) {
            if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                current_state = EXIT;
                continue;
            }
            
            input_buffer[strcspn(input_buffer, "\n")] = '\0';
            
            if (strcmp(input_buffer, "quit") == 0) {
                current_state = EXIT;
                continue;
            }
            
            valid_input = 0;
            if (sscanf(input_buffer, "%lf", &operand2) == 1) {
                valid_input = 1;
            }
            
            if (!valid_input) {
                printf("Invalid number. Please try again.\n");
                continue;
            }
            
            current_state = CALCULATE;
        }
    }
    
    printf("Goodbye!\n");
    return 0;
}