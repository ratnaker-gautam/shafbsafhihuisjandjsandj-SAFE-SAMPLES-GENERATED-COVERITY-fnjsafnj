//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    START,
    READ_NUMBER,
    READ_OPERATOR,
    READ_SECOND_NUMBER,
    CALCULATE,
    ERROR,
    DONE
};

int main(void) {
    enum State current_state = START;
    double first_number = 0.0;
    double second_number = 0.0;
    char operator = '\0';
    char input_buffer[256];
    int valid_input = 0;
    double result = 0.0;
    
    printf("Simple Calculator (type 'quit' to exit)\n");
    printf("Enter expression (e.g., 5 + 3): ");
    
    while (current_state != DONE && current_state != ERROR) {
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            current_state = ERROR;
            break;
        }
        
        size_t len = strlen(input_buffer);
        if (len > 0 && input_buffer[len - 1] == '\n') {
            input_buffer[len - 1] = '\0';
            len--;
        }
        
        if (strcmp(input_buffer, "quit") == 0) {
            current_state = DONE;
            break;
        }
        
        switch (current_state) {
            case START:
                if (sscanf(input_buffer, "%lf", &first_number) == 1) {
                    current_state = READ_OPERATOR;
                    printf("Enter operator (+, -, *, /): ");
                } else {
                    printf("Invalid number. Enter first number: ");
                }
                break;
                
            case READ_OPERATOR:
                if (strlen(input_buffer) == 1) {
                    operator = input_buffer[0];
                    if (operator == '+' || operator == '-' || operator == '*' || operator == '/') {
                        current_state = READ_SECOND_NUMBER;
                        printf("Enter second number: ");
                    } else {
                        printf("Invalid operator. Use +, -, *, or /: ");
                    }
                } else {
                    printf("Invalid operator. Use +, -, *, or /: ");
                }
                break;
                
            case READ_SECOND_NUMBER:
                if (sscanf(input_buffer, "%lf", &second_number) == 1) {
                    if (operator == '/' && second_number == 0.0) {
                        printf("Error: Division by zero. Enter new second number: ");
                    } else {
                        current_state = CALCULATE;
                    }
                } else {
                    printf("Invalid number. Enter second number: ");
                }
                break;
                
            case CALCULATE:
                switch (operator) {
                    case '+':
                        result = first_number + second_number;
                        break;
                    case '-':
                        result = first_number - second_number;
                        break;
                    case '*':
                        result = first_number * second_number;
                        break;
                    case '/':
                        result = first_number / second_number;
                        break;
                    default:
                        current_state = ERROR;
                        break;
                }
                
                if (current_state != ERROR) {
                    printf("Result: %.2f %c %.2f = %.2f\n", first_number, operator, second_number, result);
                    current_state = START;
                    printf("\nEnter new expression (or 'quit'): ");
                }
                break;
                
            case ERROR:
                printf("An error occurred. Restarting...\n");
                current_state = START;
                printf("Enter first number: ");
                break;
                
            case DONE:
                break;
        }
    }
    
    printf("Calculator terminated.\n");
    return 0;
}