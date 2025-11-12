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
    int valid_input = 0;
    double result = 0.0;

    while (1) {
        switch (current_state) {
            case START:
                printf("Simple Calculator (type 'quit' to exit)\n");
                printf("Enter first number: ");
                current_state = READ_NUM;
                break;

            case READ_NUM:
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    current_state = ERROR;
                    break;
                }
                
                size_t len = strlen(input_buffer);
                if (len > 0 && input_buffer[len - 1] == '\n') {
                    input_buffer[len - 1] = '\0';
                    len--;
                }
                
                if (len == 0) {
                    printf("Empty input. Please enter a number: ");
                    break;
                }
                
                if (strcmp(input_buffer, "quit") == 0) {
                    current_state = EXIT;
                    break;
                }
                
                char *endptr;
                first_num = strtod(input_buffer, &endptr);
                if (endptr == input_buffer || *endptr != '\0') {
                    printf("Invalid number. Please enter a valid number: ");
                    break;
                }
                
                printf("Enter operation (+, -, *, /): ");
                current_state = READ_OP;
                break;

            case READ_OP:
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    current_state = ERROR;
                    break;
                }
                
                len = strlen(input_buffer);
                if (len > 0 && input_buffer[len - 1] == '\n') {
                    input_buffer[len - 1] = '\0';
                    len--;
                }
                
                if (len != 1 || (input_buffer[0] != '+' && input_buffer[0] != '-' && 
                    input_buffer[0] != '*' && input_buffer[0] != '/')) {
                    printf("Invalid operation. Please enter +, -, *, or /: ");
                    break;
                }
                
                operation = input_buffer[0];
                printf("Enter second number: ");
                current_state = READ_SECOND;
                break;

            case READ_SECOND:
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    current_state = ERROR;
                    break;
                }
                
                len = strlen(input_buffer);
                if (len > 0 && input_buffer[len - 1] == '\n') {
                    input_buffer[len - 1] = '\0';
                    len--;
                }
                
                if (len == 0) {
                    printf("Empty input. Please enter a number: ");
                    break;
                }
                
                if (strcmp(input_buffer, "quit") == 0) {
                    current_state = EXIT;
                    break;
                }
                
                endptr = NULL;
                second_num = strtod(input_buffer, &endptr);
                if (endptr == input_buffer || *endptr != '\0') {
                    printf("Invalid number. Please enter a valid number: ");
                    break;
                }
                
                if (operation == '/' && second_num == 0.0) {
                    printf("Error: Division by zero is not allowed.\n");
                    current_state = START;
                    break;
                }
                
                current_state = CALCULATE;
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
                    printf("Result: %.2f %c %.2f = %.2f\n\n", first_num, operation, second_num, result);
                    current_state = START;
                }
                break;

            case ERROR:
                printf("An error occurred. Restarting calculator.\n\n");
                current_state = START;
                break;

            case EXIT:
                printf("Goodbye!\n");
                return 0;
        }
    }

    return 0;
}