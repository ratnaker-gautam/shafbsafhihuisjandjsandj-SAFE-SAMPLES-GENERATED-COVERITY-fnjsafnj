//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: monolithic_main ; Variation: state_machine
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
    int valid_input = 0;
    double result = 0.0;
    
    printf("Simple Calculator (+, -, *, /)\n");
    printf("Enter 'q' to quit\n");
    
    while (current_state != DONE && current_state != ERROR) {
        switch (current_state) {
            case START:
                printf("Enter first number: ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    current_state = ERROR;
                    break;
                }
                
                if (strlen(input_buffer) > 0 && input_buffer[0] == 'q') {
                    current_state = DONE;
                    break;
                }
                
                if (sscanf(input_buffer, "%lf", &first_num) == 1) {
                    current_state = READ_OP;
                } else {
                    printf("Invalid number. Please try again.\n");
                }
                break;
                
            case READ_OP:
                printf("Enter operation (+, -, *, /): ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    current_state = ERROR;
                    break;
                }
                
                if (strlen(input_buffer) >= 1) {
                    operation = input_buffer[0];
                    if (operation == '+' || operation == '-' || 
                        operation == '*' || operation == '/') {
                        current_state = READ_SECOND;
                    } else {
                        printf("Invalid operation. Please use +, -, *, or /.\n");
                    }
                } else {
                    printf("No operation entered.\n");
                }
                break;
                
            case READ_SECOND:
                printf("Enter second number: ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    current_state = ERROR;
                    break;
                }
                
                if (sscanf(input_buffer, "%lf", &second_num) == 1) {
                    current_state = CALCULATE;
                } else {
                    printf("Invalid number. Please try again.\n");
                }
                break;
                
            case CALCULATE:
                valid_input = 1;
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
                        if (second_num != 0.0) {
                            result = first_num / second_num;
                        } else {
                            printf("Error: Division by zero.\n");
                            valid_input = 0;
                        }
                        break;
                    default:
                        valid_input = 0;
                        break;
                }
                
                if (valid_input) {
                    printf("Result: %.2f %c %.2f = %.2f\n", 
                           first_num, operation, second_num, result);
                }
                
                current_state = START;
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