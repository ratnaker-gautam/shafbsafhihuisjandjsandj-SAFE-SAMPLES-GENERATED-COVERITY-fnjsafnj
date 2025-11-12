//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    INIT,
    READING_NUM,
    READING_OP,
    CALCULATING,
    ERROR,
    DONE
};

int main(void) {
    enum state current_state = INIT;
    double accumulator = 0.0;
    double current_num = 0.0;
    char current_op = '+';
    char input[256];
    int pos = 0;
    int decimal_places = 0;
    int in_decimal = 0;
    int sign = 1;
    
    printf("Simple Calculator (enter '=' to compute, 'q' to quit)\n");
    printf("Current value: 0.0\n");
    
    while (current_state != DONE && current_state != ERROR) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            current_state = ERROR;
            break;
        }
        
        for (pos = 0; input[pos] != '\0' && input[pos] != '\n'; pos++) {
            char c = input[pos];
            
            switch (current_state) {
                case INIT:
                case READING_OP:
                    if (isdigit(c)) {
                        current_num = (c - '0') * sign;
                        in_decimal = 0;
                        decimal_places = 0;
                        current_state = READING_NUM;
                    } else if (c == '+' || c == '-' || c == '*' || c == '/') {
                        current_op = c;
                        current_state = READING_OP;
                    } else if (c == '=') {
                        current_state = CALCULATING;
                    } else if (c == 'q' || c == 'Q') {
                        current_state = DONE;
                    } else if (!isspace(c)) {
                        current_state = ERROR;
                    }
                    break;
                    
                case READING_NUM:
                    if (isdigit(c)) {
                        if (in_decimal) {
                            decimal_places++;
                            double digit_value = (c - '0');
                            for (int i = 0; i < decimal_places; i++) {
                                digit_value /= 10.0;
                            }
                            current_num += digit_value * sign;
                        } else {
                            current_num = current_num * 10.0 + (c - '0') * sign;
                        }
                    } else if (c == '.') {
                        if (in_decimal) {
                            current_state = ERROR;
                        } else {
                            in_decimal = 1;
                        }
                    } else if (c == '+' || c == '-' || c == '*' || c == '/') {
                        switch (current_op) {
                            case '+': accumulator += current_num; break;
                            case '-': accumulator -= current_num; break;
                            case '*': accumulator *= current_num; break;
                            case '/': 
                                if (current_num == 0.0) {
                                    current_state = ERROR;
                                } else {
                                    accumulator /= current_num;
                                }
                                break;
                        }
                        if (current_state != ERROR) {
                            current_op = c;
                            current_state = READING_OP;
                        }
                    } else if (c == '=') {
                        switch (current_op) {
                            case '+': accumulator += current_num; break;
                            case '-': accumulator -= current_num; break;
                            case '*': accumulator *= current_num; break;
                            case '/': 
                                if (current_num == 0.0) {
                                    current_state = ERROR;
                                } else {
                                    accumulator /= current_num;
                                }
                                break;
                        }
                        if (current_state != ERROR) {
                            current_state = CALCULATING;
                        }
                    } else if (c == 'q' || c == 'Q') {
                        current_state = DONE;
                    } else if (!isspace(c)) {
                        current_state = ERROR;
                    }
                    break;
                    
                case CALCULATING:
                    printf("Result: %.6f\n", accumulator);
                    printf("Enter new operation or 'q' to quit: ");
                    current_state = INIT;
                    current_op = '+';
                    current_num = 0.0;
                    break;
                    
                default:
                    break;
            }
            
            if (current_state == ERROR || current_state == DONE) {
                break;
            }
        }
    }
    
    if (current_state == ERROR) {
        printf("Error: Invalid input\n");
        return 1;
    }
    
    printf("Final result: %.6f\n", accumulator);
    return 0;
}