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
    char ch;
    
    printf("Simple Calculator (Enter '=' to calculate, 'q' to quit)\n");
    printf("Enter expression: ");
    
    while (state != DONE && state != ERROR) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            state = ERROR;
            break;
        }
        
        pos = 0;
        while (state != DONE && state != ERROR && (ch = input[pos++]) != '\0' && ch != '\n') {
            switch (state) {
                case INIT:
                    if (isdigit(ch)) {
                        current_number = ch - '0';
                        state = READING_NUMBER;
                    } else if (ch == 'q' || ch == 'Q') {
                        state = DONE;
                    } else if (isspace(ch)) {
                        continue;
                    } else {
                        state = ERROR;
                    }
                    break;
                    
                case READING_NUMBER:
                    if (isdigit(ch)) {
                        if (has_decimal) {
                            decimal_place *= 10.0;
                            current_number = current_number + (ch - '0') / decimal_place;
                        } else {
                            current_number = current_number * 10.0 + (ch - '0');
                        }
                    } else if (ch == '.') {
                        if (has_decimal) {
                            state = ERROR;
                        } else {
                            has_decimal = 1;
                            decimal_place = 1.0;
                        }
                    } else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
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
                        
                        current_number = 0.0;
                        has_decimal = 0;
                        decimal_place = 1.0;
                        
                        switch (ch) {
                            case '+': current_op = OP_ADD; break;
                            case '-': current_op = OP_SUBTRACT; break;
                            case '*': current_op = OP_MULTIPLY; break;
                            case '/': current_op = OP_DIVIDE; break;
                            default: state = ERROR; break;
                        }
                        state = READING_OPERATOR;
                    } else if (ch == '=') {
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
                        printf("Result: %.6f\n", accumulator);
                        state = INIT;
                        current_op = OP_NONE;
                        accumulator = 0.0;
                        current_number = 0.0;
                        has_decimal = 0;
                        decimal_place = 1.0;
                        printf("Enter expression: ");
                    } else if (isspace(ch)) {
                        continue;
                    } else {
                        state = ERROR;
                    }
                    break;
                    
                case READING_OPERATOR:
                    if (isdigit(ch)) {
                        current_number = ch - '0';
                        state = READING_NUMBER;
                    } else if (isspace(ch)) {
                        continue;
                    } else {
                        state = ERROR;
                    }
                    break;
                    
                case CALCULATING:
                    state = ERROR;
                    break;
                    
                case ERROR:
                    break;
                    
                case DONE:
                    break;
            }
        }
        
        if (state == ERROR) {
            printf("Error: Invalid expression\n");
            state = INIT;
            current_op = OP_NONE;
            accumulator = 0.0;
            current_number = 0.0;
            has_decimal = 0;
            decimal_place = 1.0;
            printf("Enter expression: ");
        }
    }
    
    printf("Calculator terminated.\n");
    return 0;
}