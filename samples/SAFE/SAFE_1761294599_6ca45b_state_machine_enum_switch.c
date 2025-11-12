//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
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

enum Operator {
    OP_NONE,
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE
};

int main(void) {
    enum State current_state = INIT;
    enum Operator current_op = OP_NONE;
    double accumulator = 0.0;
    double current_number = 0.0;
    int has_decimal = 0;
    double decimal_place = 1.0;
    char input_buffer[256];
    int buffer_pos = 0;
    int c;
    
    printf("Simple Calculator (Enter '=' to compute, 'q' to quit)\n");
    printf("Enter expression: ");
    
    while (current_state != DONE && current_state != ERROR) {
        if (buffer_pos >= 255) {
            current_state = ERROR;
            break;
        }
        
        c = getchar();
        if (c == EOF) {
            current_state = ERROR;
            break;
        }
        
        switch (current_state) {
            case INIT:
                if (isdigit(c)) {
                    current_number = c - '0';
                    current_state = READING_NUMBER;
                } else if (c == '+' || c == '-' || c == '*' || c == '/') {
                    accumulator = 0.0;
                    current_state = ERROR;
                } else if (c == 'q' || c == 'Q') {
                    current_state = DONE;
                } else if (!isspace(c) && c != '=') {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit(c)) {
                    if (has_decimal) {
                        decimal_place *= 0.1;
                        current_number += (c - '0') * decimal_place;
                    } else {
                        current_number = current_number * 10 + (c - '0');
                    }
                } else if (c == '.') {
                    if (has_decimal) {
                        current_state = ERROR;
                    } else {
                        has_decimal = 1;
                        decimal_place = 1.0;
                    }
                } else if (c == '+' || c == '-' || c == '*' || c == '/') {
                    if (current_op == OP_NONE) {
                        accumulator = current_number;
                    } else {
                        switch (current_op) {
                            case OP_ADD:
                                accumulator += current_number;
                                break;
                            case OP_SUBTRACT:
                                accumulator -= current_number;
                                break;
                            case OP_MULTIPLY:
                                accumulator *= current_number;
                                break;
                            case OP_DIVIDE:
                                if (current_number == 0.0) {
                                    current_state = ERROR;
                                    break;
                                }
                                accumulator /= current_number;
                                break;
                            default:
                                current_state = ERROR;
                                break;
                        }
                    }
                    
                    current_number = 0.0;
                    has_decimal = 0;
                    decimal_place = 1.0;
                    
                    switch (c) {
                        case '+': current_op = OP_ADD; break;
                        case '-': current_op = OP_SUBTRACT; break;
                        case '*': current_op = OP_MULTIPLY; break;
                        case '/': current_op = OP_DIVIDE; break;
                        default: current_state = ERROR; break;
                    }
                    
                    current_state = READING_OPERATOR;
                } else if (c == '=') {
                    if (current_op != OP_NONE) {
                        switch (current_op) {
                            case OP_ADD:
                                accumulator += current_number;
                                break;
                            case OP_SUBTRACT:
                                accumulator -= current_number;
                                break;
                            case OP_MULTIPLY:
                                accumulator *= current_number;
                                break;
                            case OP_DIVIDE:
                                if (current_number == 0.0) {
                                    current_state = ERROR;
                                    break;
                                }
                                accumulator /= current_number;
                                break;
                            default:
                                current_state = ERROR;
                                break;
                        }
                    } else {
                        accumulator = current_number;
                    }
                    printf("Result: %.6g\n", accumulator);
                    current_state = DONE;
                } else if (c == 'q' || c == 'Q') {
                    current_state = DONE;
                } else if (!isspace(c)) {
                    current_state = ERROR;
                }
                break;
                
            case READING_OPERATOR:
                if (isdigit(c)) {
                    current_number = c - '0';
                    current_state = READING_NUMBER;
                } else if (c == 'q' || c == 'Q') {
                    current_state = DONE;
                } else if (!isspace(c) && c != '=') {
                    current_state = ERROR;
                }
                break;
                
            case CALCULATING:
                current_state = DONE;
                break;
                
            case ERROR:
                break;
                
            case DONE:
                break;
        }
        
        if (c != '\n' && current_state != DONE && current_state != ERROR) {
            if (buffer_pos < 255) {
                input_buffer[buffer_pos++] = c;
            }
        }
    }
    
    if (current_state == ERROR) {
        printf("Error: Invalid expression\n");
        return 1;
    }
    
    return 0;
}