//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    INIT,
    READING_NUMBER,
    READING_OPERATOR,
    CALCULATING,
    ERROR,
    DONE
};

int main(void) {
    enum State current_state = INIT;
    double accumulator = 0.0;
    double current_number = 0.0;
    char operator = '+';
    char input_buffer[256];
    int buffer_index = 0;
    int decimal_places = 0;
    int in_decimal = 0;
    int sign = 1;
    
    printf("Simple Calculator (enter '=' to compute, 'q' to quit)\n");
    printf("Enter numbers and operators (+, -, *, /): ");
    
    while (current_state != DONE && current_state != ERROR) {
        int c = getchar();
        
        if (c == EOF || c == 'q' || c == 'Q') {
            current_state = DONE;
            continue;
        }
        
        switch (current_state) {
            case INIT:
                if (isspace(c)) {
                    continue;
                } else if (isdigit(c) || c == '.' || c == '+' || c == '-') {
                    buffer_index = 0;
                    current_number = 0.0;
                    decimal_places = 0;
                    in_decimal = 0;
                    sign = 1;
                    
                    if (c == '+') {
                        sign = 1;
                        current_state = READING_NUMBER;
                    } else if (c == '-') {
                        sign = -1;
                        current_state = READING_NUMBER;
                    } else if (c == '.') {
                        in_decimal = 1;
                        decimal_places = 1;
                        current_state = READING_NUMBER;
                    } else if (isdigit(c)) {
                        current_number = (c - '0');
                        current_state = READING_NUMBER;
                    }
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit(c)) {
                    if (in_decimal) {
                        current_number = current_number + (c - '0') / pow(10.0, decimal_places);
                        decimal_places++;
                    } else {
                        current_number = current_number * 10 + (c - '0');
                    }
                } else if (c == '.' && !in_decimal) {
                    in_decimal = 1;
                    decimal_places = 1;
                } else if (isspace(c)) {
                    current_number *= sign;
                    accumulator = current_number;
                    current_state = READING_OPERATOR;
                } else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '=') {
                    current_number *= sign;
                    
                    switch (operator) {
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
                        default: accumulator = current_number; break;
                    }
                    
                    if (current_state != ERROR) {
                        if (c == '=') {
                            current_state = DONE;
                        } else {
                            operator = c;
                            current_state = READING_OPERATOR;
                        }
                    }
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_OPERATOR:
                if (isspace(c)) {
                    continue;
                } else if (isdigit(c) || c == '.' || c == '+' || c == '-') {
                    buffer_index = 0;
                    current_number = 0.0;
                    decimal_places = 0;
                    in_decimal = 0;
                    sign = 1;
                    
                    if (c == '+') {
                        sign = 1;
                        current_state = READING_NUMBER;
                    } else if (c == '-') {
                        sign = -1;
                        current_state = READING_NUMBER;
                    } else if (c == '.') {
                        in_decimal = 1;
                        decimal_places = 1;
                        current_state = READING_NUMBER;
                    } else if (isdigit(c)) {
                        current_number = (c - '0');
                        current_state = READING_NUMBER;
                    }
                } else if (c == '=') {
                    current_state = DONE;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case CALCULATING:
                break;
                
            case ERROR:
                break;
                
            case DONE:
                break;
        }
    }
    
    if (current_state == ERROR) {
        printf("\nError: Invalid input sequence\n");
        return 1;
    } else if (current_state == DONE) {
        printf("\nResult: %.6f\n", accumulator);
    }
    
    return 0;
}