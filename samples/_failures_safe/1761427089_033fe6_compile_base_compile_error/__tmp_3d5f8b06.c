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
    char input[256];
    int input_len;
    int i = 0;
    
    printf("Simple Calculator (enter '=' to compute, 'q' to quit)\n");
    printf("Enter expression: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }
    
    input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    while (i < input_len && current_state != ERROR && current_state != DONE) {
        char c = input[i];
        
        switch (current_state) {
            case INIT:
                if (isdigit(c)) {
                    current_number = c - '0';
                    current_state = READING_NUMBER;
                } else if (c == '+' || c == '-' || c == '*' || c == '/') {
                    current_state = ERROR;
                } else if (c == '=') {
                    accumulator = 0.0;
                    current_state = DONE;
                } else if (c == 'q' || c == 'Q') {
                    return 0;
                } else if (!isspace(c)) {
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
                                } else {
                                    accumulator /= current_number;
                                }
                                break;
                            default:
                                current_state = ERROR;
                                break;
                        }
                    }
                    
                    if (current_state != ERROR) {
                        switch (c) {
                            case '+': current_op = OP_ADD; break;
                            case '-': current_op = OP_SUBTRACT; break;
                            case '*': current_op = OP_MULTIPLY; break;
                            case '/': current_op = OP_DIVIDE; break;
                            default: current_state = ERROR; break;
                        }
                        current_number = 0.0;
                        has_decimal = 0;
                        decimal_place = 1.0;
                        current_state = READING_OPERATOR;
                    }
                } else if (c == '=') {
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
                                } else {
                                    accumulator /= current_number;
                                }
                                break;
                            default:
                                current_state = ERROR;
                                break;
                        }
                    }
                    current_state = DONE;
                } else if (c == 'q' || c == 'Q') {
                    return 0;
                } else if (!isspace(c)) {
                    current_state = ERROR;
                }
                break;
                
            case READING_OPERATOR:
                if (isdigit(c)) {
                    current_number = c - '0';
                    current_state = READING_NUMBER;
                } else if (c == '.') {
                    current_number = 0.0;
                    has_decimal = 1;
                    decimal_place = 1.0;
                    current_state = READING_NUMBER;
                } else if (c == '=') {
                    current_state = DONE;
                } else if (c == 'q' || c == 'Q') {
                    return 0;
                } else if (!isspace(c)) {
                    current_state = ERROR;
                }
                break;
                
            case CALCULATING:
                current_state = ERROR;
                break;
                
            case ERROR:
                break;
                
            case DONE:
                break;
        }
        
        i++;
    }
    
    if (current