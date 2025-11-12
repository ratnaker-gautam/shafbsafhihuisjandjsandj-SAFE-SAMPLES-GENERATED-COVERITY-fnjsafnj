//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    INIT,
    READING_NUMBER,
    READING_OPERATOR,
    FINISHED,
    ERROR
};

int main(void) {
    enum State current_state = INIT;
    double accumulator = 0.0;
    double current_number = 0.0;
    char current_operator = '+';
    char input_buffer[256];
    int position = 0;
    int decimal_places = 0;
    int in_decimal = 0;
    int valid_input = 1;
    
    printf("Simple Calculator (enter '=' to finish, 'q' to quit)\n");
    printf("Enter expression: ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    while (position < input_len && current_state != FINISHED && current_state != ERROR) {
        char c = input_buffer[position];
        
        switch (current_state) {
            case INIT:
                if (isdigit(c)) {
                    current_number = c - '0';
                    current_state = READING_NUMBER;
                } else if (c == '+' || c == '-') {
                    current_operator = c;
                    current_state = READING_OPERATOR;
                } else if (c == '=') {
                    current_state = FINISHED;
                } else if (c == 'q' || c == 'Q') {
                    return 0;
                } else if (!isspace(c)) {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit(c)) {
                    if (in_decimal) {
                        decimal_places++;
                        double digit_value = (c - '0');
                        for (int i = 0; i < decimal_places; i++) {
                            digit_value /= 10.0;
                        }
                        current_number += digit_value;
                    } else {
                        current_number = current_number * 10.0 + (c - '0');
                    }
                } else if (c == '.') {
                    if (in_decimal) {
                        current_state = ERROR;
                    } else {
                        in_decimal = 1;
                        decimal_places = 0;
                    }
                } else if (c == '+' || c == '-' || c == '*' || c == '/') {
                    switch (current_operator) {
                        case '+': accumulator += current_number; break;
                        case '-': accumulator -= current_number; break;
                        case '*': accumulator *= current_number; break;
                        case '/': 
                            if (current_number != 0.0) {
                                accumulator /= current_number;
                            } else {
                                current_state = ERROR;
                            }
                            break;
                    }
                    if (current_state != ERROR) {
                        current_operator = c;
                        current_number = 0.0;
                        in_decimal = 0;
                        decimal_places = 0;
                        current_state = READING_OPERATOR;
                    }
                } else if (c == '=') {
                    switch (current_operator) {
                        case '+': accumulator += current_number; break;
                        case '-': accumulator -= current_number; break;
                        case '*': accumulator *= current_number; break;
                        case '/': 
                            if (current_number != 0.0) {
                                accumulator /= current_number;
                            } else {
                                current_state = ERROR;
                            }
                            break;
                    }
                    current_state = FINISHED;
                } else if (!isspace(c)) {
                    current_state = ERROR;
                }
                break;
                
            case READING_OPERATOR:
                if (isdigit(c)) {
                    current_number = c - '0';
                    current_state = READING_NUMBER;
                } else if (c == '=') {
                    current_state = FINISHED;
                } else if (!isspace(c)) {
                    current_state = ERROR;
                }
                break;
                
            default:
                break;
        }
        
        position++;
    }
    
    if (current_state == FINISHED) {
        printf("Result: %.6f\n", accumulator);
    } else if (current_state == ERROR) {
        printf("Error: Invalid expression\n");
        return 1;
    } else {
        printf("Error: Incomplete expression\n");
        return 1;
    }
    
    return 0;
}