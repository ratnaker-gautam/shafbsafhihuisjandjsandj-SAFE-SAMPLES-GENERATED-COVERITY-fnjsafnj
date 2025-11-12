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
    int buffer_pos = 0;
    int valid_input = 1;
    
    printf("Simple Calculator (enter 'quit' to exit)\n");
    printf("Enter expression (e.g., 5 + 3): ");
    
    while (current_state != DONE && current_state != ERROR) {
        int c = getchar();
        
        if (c == EOF) {
            current_state = ERROR;
            break;
        }
        
        switch (current_state) {
            case START:
                if (isdigit(c) || c == '+' || c == '-') {
                    if (buffer_pos < 255) {
                        buffer[buffer_pos++] = c;
                        current_state = READ_NUMBER;
                    } else {
                        current_state = ERROR;
                    }
                } else if (isspace(c)) {
                    continue;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READ_NUMBER:
                if (isdigit(c) || c == '.') {
                    if (buffer_pos < 255) {
                        buffer[buffer_pos++] = c;
                    } else {
                        current_state = ERROR;
                    }
                } else if (isspace(c)) {
                    buffer[buffer_pos] = '\0';
                    char *endptr;
                    first_number = strtod(buffer, &endptr);
                    if (endptr == buffer || *endptr != '\0') {
                        current_state = ERROR;
                    } else {
                        buffer_pos = 0;
                        current_state = READ_OPERATOR;
                    }
                } else if (c == '+' || c == '-' || c == '*' || c == '/') {
                    buffer[buffer_pos] = '\0';
                    char *endptr;
                    first_number = strtod(buffer, &endptr);
                    if (endptr == buffer || *endptr != '\0') {
                        current_state = ERROR;
                    } else {
                        operator = c;
                        buffer_pos = 0;
                        current_state = READ_SECOND_NUMBER;
                    }
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READ_OPERATOR:
                if (c == '+' || c == '-' || c == '*' || c == '/') {
                    operator = c;
                    current_state = READ_SECOND_NUMBER;
                } else if (isspace(c)) {
                    continue;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READ_SECOND_NUMBER:
                if (isdigit(c) || c == '+' || c == '-' || c == '.') {
                    if (buffer_pos < 255) {
                        buffer[buffer_pos++] = c;
                    } else {
                        current_state = ERROR;
                    }
                } else if (c == '\n' || c == '=') {
                    buffer[buffer_pos] = '\0';
                    char *endptr;
                    second_number = strtod(buffer, &endptr);
                    if (endptr == buffer || *endptr != '\0') {
                        current_state = ERROR;
                    } else {
                        current_state = CALCULATE;
                    }
                } else if (isspace(c)) {
                    continue;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case CALCULATE:
                {
                    double result = 0.0;
                    int calculation_valid = 1;
                    
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
                            if (second_number == 0.0) {
                                calculation_valid = 0;
                            } else {
                                result = first_number / second_number;
                            }
                            break;
                        default:
                            calculation_valid = 0;
                            break;
                    }
                    
                    if (calculation_valid) {
                        printf("Result: %.2f\n", result);
                    } else {
                        printf("Error: Invalid calculation\n");
                    }
                    
                    printf("Enter another expression (or 'quit'): ");
                    current_state = START;
                    first_number = 0.0;
                    second_number = 0.0;
                    operator = '\0';
                    buffer_pos = 0;
                    
                    while ((c = getchar()) != '\n' && c != EOF) {
                        if (buffer_pos < 255) {
                            buffer[buffer_pos++] = c;
                        }
                    }
                    buffer[buffer_pos] = '\0';
                    
                    if (strncmp(buffer, "quit", 4) == 0) {
                        current_state = DONE;
                    } else {
                        buffer_pos = 0;
                    }
                }
                break;
                
            case ERROR:
                printf("Error: Invalid input format\n");
                printf("Enter expression (e.g., 5 + 3): ");
                current_state = START;
                first_number = 0.0;
                second_number = 0.0;
                operator = '\0';
                buffer_pos = 0;
                
                while ((c = getchar()) != '\n' && c != EOF) {
                    continue;
                }
                break;
                
            default