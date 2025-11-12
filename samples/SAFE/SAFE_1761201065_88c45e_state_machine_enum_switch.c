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
    int pos = 0;
    char ch;
    
    printf("Simple Calculator (enter '=' to calculate, 'q' to quit)\n");
    printf("Enter expression: ");
    
    while (current_state != DONE && current_state != ERROR) {
        if (pos >= 255) {
            current_state = ERROR;
            break;
        }
        
        int result = scanf("%c", &ch);
        if (result != 1) {
            current_state = ERROR;
            break;
        }
        
        if (ch == '\n') {
            continue;
        }
        
        input[pos++] = ch;
        input[pos] = '\0';
        
        switch (current_state) {
            case INIT:
                if (isdigit(ch)) {
                    current_number = ch - '0';
                    current_state = READING_NUMBER;
                } else if (ch == 'q' || ch == 'Q') {
                    current_state = DONE;
                } else {
                    current_state = ERROR;
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
                        current_state = ERROR;
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
                    
                    switch (ch) {
                        case '+':
                            current_op = OP_ADD;
                            break;
                        case '-':
                            current_op = OP_SUBTRACT;
                            break;
                        case '*':
                            current_op = OP_MULTIPLY;
                            break;
                        case '/':
                            current_op = OP_DIVIDE;
                            break;
                        default:
                            current_state = ERROR;
                            break;
                    }
                    current_state = READING_OPERATOR;
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
                    printf("Result: %.6f\n", accumulator);
                    current_state = DONE;
                } else if (ch == 'q' || ch == 'Q') {
                    current_state = DONE;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_OPERATOR:
                if (isdigit(ch)) {
                    current_number = ch - '0';
                    current_state = READING_NUMBER;
                } else if (ch == 'q' || ch == 'Q') {
                    current_state = DONE;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case CALCULATING:
            case ERROR:
            case DONE:
                break;
        }
    }
    
    if (current_state == ERROR) {
        printf("Error: Invalid expression\n");
        return 1;
    }
    
    return 0;
}