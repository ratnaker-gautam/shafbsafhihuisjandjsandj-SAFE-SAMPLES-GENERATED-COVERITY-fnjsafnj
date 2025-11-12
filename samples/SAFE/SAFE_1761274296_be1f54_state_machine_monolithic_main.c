//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    START,
    READ_NUM,
    READ_OP,
    READ_SECOND,
    CALCULATE,
    ERROR,
    EXIT
};

int main(void) {
    enum State current_state = START;
    double first_num = 0.0;
    double second_num = 0.0;
    char operation = '\0';
    char input_buffer[256];
    double result = 0.0;
    
    printf("Simple Calculator (type 'quit' to exit)\n");
    
    while (1) {
        switch (current_state) {
            case START:
                printf("Enter first number: ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    current_state = ERROR;
                    break;
                }
                
                if (strncmp(input_buffer, "quit", 4) == 0) {
                    current_state = EXIT;
                    break;
                }
                
                if (sscanf(input_buffer, "%lf", &first_num) == 1) {
                    current_state = READ_OP;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READ_OP:
                printf("Enter operation (+, -, *, /): ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    current_state = ERROR;
                    break;
                }
                
                if (strlen(input_buffer) >= 2) {
                    operation = input_buffer[0];
                    if (operation == '+' || operation == '-' || operation == '*' || operation == '/') {
                        current_state = READ_SECOND;
                    } else {
                        current_state = ERROR;
                    }
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READ_SECOND:
                printf("Enter second number: ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    current_state = ERROR;
                    break;
                }
                
                if (sscanf(input_buffer, "%lf", &second_num) == 1) {
                    if (operation == '/' && second_num == 0.0) {
                        current_state = ERROR;
                    } else {
                        current_state = CALCULATE;
                    }
                } else {
                    current_state = ERROR;
                }
                break;
                
            case CALCULATE:
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
                        result = first_num / second_num;
                        break;
                    default:
                        current_state = ERROR;
                        break;
                }
                
                if (current_state != ERROR) {
                    printf("Result: %.2f\n", result);
                    current_state = START;
                }
                break;
                
            case ERROR:
                printf("Error: Invalid input or operation\n");
                current_state = START;
                break;
                
            case EXIT:
                printf("Goodbye!\n");
                return 0;
        }
    }
    
    return 0;
}