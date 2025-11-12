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
    int parse_result = 0;
    char *endptr = NULL;

    printf("Simple Calculator (type 'quit' to exit)\n");
    printf("Current value: 0.0\n");

    while (current_state != EXIT) {
        switch (current_state) {
            case INIT:
                printf("Enter number: ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    current_state = ERROR;
                    break;
                }
                input_buffer[strcspn(input_buffer, "\n")] = '\0';
                
                if (strcmp(input_buffer, "quit") == 0) {
                    current_state = EXIT;
                    break;
                }
                
                operand = strtod(input_buffer, &endptr);
                if (endptr == input_buffer || *endptr != '\0') {
                    current_state = ERROR;
                } else {
                    current_state = READ_OP;
                }
                break;

            case READ_OP:
                printf("Enter operator (+, -, *, /): ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    current_state = ERROR;
                    break;
                }
                input_buffer[strcspn(input_buffer, "\n")] = '\0';
                
                if (strlen(input_buffer) != 1) {
                    current_state = ERROR;
                    break;
                }
                
                operator = input_buffer[0];
                if (operator != '+' && operator != '-' && operator != '*' && operator != '/') {
                    current_state = ERROR;
                } else {
                    current_state = CALCULATE;
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
                        if (operand == 0.0) {
                            current_state = ERROR;
                            break;
                        }
                        accumulator /= operand;
                        break;
                    default:
                        current_state = ERROR;
                        break;
                }
                
                if (current_state != ERROR) {
                    printf("Current value: %.2f\n", accumulator);
                    current_state = INIT;
                }
                break;

            case ERROR:
                printf("Error: Invalid input or operation\n");
                printf("Resetting calculator...\n");
                accumulator = 0.0;
                printf("Current value: 0.0\n");
                current_state = INIT;
                break;

            case EXIT:
                break;
        }
    }

    printf("Final result: %.2f\n", accumulator);
    printf("Goodbye!\n");
    return 0;
}