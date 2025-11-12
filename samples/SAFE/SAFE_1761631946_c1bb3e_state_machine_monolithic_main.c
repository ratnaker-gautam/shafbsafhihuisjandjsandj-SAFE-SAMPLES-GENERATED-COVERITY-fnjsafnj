//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    START,
    READ_NUM,
    READ_OP,
    READ_SECOND,
    CALCULATE,
    ERROR,
    DONE
};

int main(void) {
    enum state current_state = START;
    double first_num = 0.0;
    double second_num = 0.0;
    char operation = '\0';
    char input_buffer[256];
    char *endptr;
    int valid_input = 0;
    
    printf("Simple Calculator (enter 'q' to quit)\n");
    
    while (current_state != DONE && current_state != ERROR) {
        switch (current_state) {
            case START:
                printf("Enter first number: ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    current_state = ERROR;
                    break;
                }
                
                if (strlen(input_buffer) > 0 && input_buffer[strlen(input_buffer)-1] == '\n') {
                    input_buffer[strlen(input_buffer)-1] = '\0';
                }
                
                if (strcmp(input_buffer, "q") == 0) {
                    current_state = DONE;
                    break;
                }
                
                first_num = strtod(input_buffer, &endptr);
                if (endptr == input_buffer || *endptr != '\0') {
                    printf("Invalid number. Please try again.\n");
                    current_state = START;
                } else {
                    current_state = READ_OP;
                }
                break;
                
            case READ_OP:
                printf("Enter operation (+, -, *, /): ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    current_state = ERROR;
                    break;
                }
                
                if (strlen(input_buffer) > 0 && input_buffer[strlen(input_buffer)-1] == '\n') {
                    input_buffer[strlen(input_buffer)-1] = '\0';
                }
                
                if (strcmp(input_buffer, "q") == 0) {
                    current_state = DONE;
                    break;
                }
                
                if (strlen(input_buffer) != 1 || 
                    (input_buffer[0] != '+' && input_buffer[0] != '-' && 
                     input_buffer[0] != '*' && input_buffer[0] != '/')) {
                    printf("Invalid operation. Please use +, -, *, or /.\n");
                    current_state = READ_OP;
                } else {
                    operation = input_buffer[0];
                    current_state = READ_SECOND;
                }
                break;
                
            case READ_SECOND:
                printf("Enter second number: ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    current_state = ERROR;
                    break;
                }
                
                if (strlen(input_buffer) > 0 && input_buffer[strlen(input_buffer)-1] == '\n') {
                    input_buffer[strlen(input_buffer)-1] = '\0';
                }
                
                if (strcmp(input_buffer, "q") == 0) {
                    current_state = DONE;
                    break;
                }
                
                second_num = strtod(input_buffer, &endptr);
                if (endptr == input_buffer || *endptr != '\0') {
                    printf("Invalid number. Please try again.\n");
                    current_state = READ_SECOND;
                } else {
                    current_state = CALCULATE;
                }
                break;
                
            case CALCULATE:
                {
                    double result = 0.0;
                    int calculation_error = 0;
                    
                    switch (operation) {
                        case '+':
                            result = first_num + second_num;
                            break;
                        case '-':
                            result = first_num - second_num;
                            break;
                        case '*':
                            result = first_num * second_num;
                            break;
                        case '/':
                            if (second_num == 0.0) {
                                printf("Error: Division by zero.\n");
                                calculation_error = 1;
                            } else {
                                result = first_num / second_num;
                            }
                            break;
                        default:
                            calculation_error = 1;
                            break;
                    }
                    
                    if (!calculation_error) {
                        printf("Result: %.2f %c %.2f = %.2f\n", 
                               first_num, operation, second_num, result);
                    }
                    
                    current_state = START;
                }
                break;
                
            case ERROR:
                printf("An error occurred. Exiting.\n");
                current_state = DONE;
                break;
                
            case DONE:
                break;
        }
    }
    
    printf("Goodbye!\n");
    return 0;
}