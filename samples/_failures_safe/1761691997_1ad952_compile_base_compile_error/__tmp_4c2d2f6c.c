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
    enum State state = INIT;
    enum Operator current_op = OP_NONE;
    double accumulator = 0.0;
    double current_number = 0.0;
    int has_decimal = 0;
    double decimal_place = 1.0;
    char input[256];
    int pos = 0;
    char c;
    
    printf("Simple Calculator (enter '=' to calculate, 'q' to quit)\n");
    printf("Enter expression: ");
    
    while (state != DONE && state != ERROR) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            state = ERROR;
            break;
        }
        
        pos = 0;
        while (input[pos] != '\0' && input[pos] != '\n') {
            c = input[pos++];
            
            switch (state) {
                case INIT:
                    if (isdigit(c)) {
                        current_number = c - '0';
                        state = READING_NUMBER;
                        has_decimal = 0;
                        decimal_place = 1.0;
                    } else if (c == 'q' || c == 'Q') {
                        state = DONE;
                    } else if (isspace(c)) {
                        continue;
                    } else {
                        state = ERROR;
                    }
                    break;
                    
                case READING_NUMBER:
                    if (isdigit(c)) {
                        if (has_decimal) {
                            decimal_place *= 10.0;
                            current_number = current_number + (c - '0') / decimal_place;
                        } else {
                            current_number = current_number * 10.0 + (c - '0');
                        }
                    } else if (c == '.') {
                        if (has_decimal) {
                            state = ERROR;
                        } else {
                            has_decimal = 1;
                        }
                    } else if (isspace(c)) {
                        state = READING_OPERATOR;
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
                                        state = ERROR;
                                        break;
                                    }
                                    accumulator /= current_number;
                                    break;
                                default:
                                    state = ERROR;
                                    break;
                            }
                        }
                        
                        switch (c) {
                            case '+': current_op = OP_ADD; break;
                            case '-': current_op = OP_SUBTRACT; break;
                            case '*': current_op = OP_MULTIPLY; break;
                            case '/': current_op = OP_DIVIDE; break;
                            default: state = ERROR; break;
                        }
                        
                        current_number = 0.0;
                        has_decimal = 0;
                        decimal_place = 1.0;
                        state = READING_NUMBER;
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
                                        state = ERROR;
                                        break;
                                    }
                                    accumulator /= current_number;
                                    break;
                                default:
                                    state = ERROR;
                                    break;
                            }
                        } else {
                            accumulator = current_number;
                        }
                        printf("Result: %.6g\n", accumulator);
                        state = INIT;
                        current_op = OP_NONE;
                        accumulator = 0.0;
                        current_number = 0.0;
                        has_decimal = 0;
                        decimal_place = 1.0;
                        printf("Enter expression: ");
                    } else {
                        state = ERROR;
                    }
                    break;
                    
                case READING_OPERATOR:
                    if (c == '+' || c == '-' || c == '*' || c == '/') {
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
                                        state = ERROR;
                                        break;
                                    }
                                    accumulator /= current_number;
                                    break;
                                default:
                                    state = ERROR;
                                    break;
                            }
                        }
                        
                        switch (c) {
                            case '+': current_op = OP_ADD; break;
                            case '-': current_op = OP_SUBTRACT; break;
                            case '*': current_op = OP_MULTIPLY; break;
                            case '/': current_op = OP_DIVIDE; break;
                            default: