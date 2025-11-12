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
    char buffer[256];
    char *endptr;
    int valid_input = 1;
    
    printf("Simple Calculator (enter 'quit' to exit)\n");
    printf("Enter expression (e.g., 5 + 3): ");
    
    while (current_state != DONE && current_state != ERROR) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            current_state = ERROR;
            break;
        }
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
            len--;
        }
        
        if (strcmp(buffer, "quit") == 0) {
            current_state = DONE;
            break;
        }
        
        switch (current_state) {
            case START:
                first_number = strtod(buffer, &endptr);
                if (endptr != buffer && *endptr == '\0') {
                    current_state = READ_OPERATOR;
                    printf("Enter operator (+, -, *, /): ");
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READ_OPERATOR:
                if (strlen(buffer) == 1 && (buffer[0] == '+' || buffer[0] == '-' || 
                    buffer[0] == '*' || buffer[0] == '/')) {
                    operator = buffer[0];
                    current_state = READ_SECOND_NUMBER;
                    printf("Enter second number: ");
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READ_SECOND_NUMBER:
                second_number = strtod(buffer, &endptr);
                if (endptr != buffer && *endptr == '\0') {
                    current_state = CALCULATE;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case CALCULATE:
                {
                    double result = 0.0;
                    valid_input = 1;
                    
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
                            if (second_number != 0.0) {
                                result = first_number / second_number;
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
                        printf("Result: %.2f\n", result);
                    }
                    
                    current_state = START;
                    printf("\nEnter expression (enter 'quit' to exit): ");
                }
                break;
                
            case ERROR:
                printf("Invalid input. Please try again.\n");
                current_state = START;
                printf("Enter expression (enter 'quit' to exit): ");
                break;
                
            default:
                current_state = ERROR;
                break;
        }
    }
    
    printf("Goodbye!\n");
    return 0;
}