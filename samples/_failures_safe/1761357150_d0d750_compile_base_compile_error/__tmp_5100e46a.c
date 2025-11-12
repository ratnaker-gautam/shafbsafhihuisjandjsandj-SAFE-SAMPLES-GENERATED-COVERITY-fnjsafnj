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
    char c;
    
    printf("Simple Calculator (enter '=' to compute, 'q' to quit)\n");
    printf("Enter expression: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    
    input_len = strlen(input);
    if (input_len <= 1) {
        return EXIT_FAILURE;
    }
    
    while (i < input_len && current_state != ERROR && current_state != DONE) {
        c = input[i];
        
        switch (current_state) {
            case INIT:
                if (isdigit(c)) {
                    current_number = c - '0';
                    current_state = READING_NUMBER;
                } else if (c == '+' || c == '-' || c == '*' || c == '/') {
                    printf("Error: Operator at start\n");
                    current_state = ERROR;
                } else if (c == '=') {
                    printf("Result: 0\n");
                    current_state = DONE;
                } else if (c == 'q' || c == 'Q') {
                    current_state = DONE;
                } else if (!isspace(c)) {
                    printf("Error: Invalid character at start\n");
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit(c)) {
                    if (has_decimal) {
                        decimal_place *= 10.0;
                        current_number = current_number + (c - '0') / decimal_place;
                    } else {
                        current_number = current_number * 10 + (c - '0');
                    }
                } else if (c == '.') {
                    if (has_decimal) {
                        printf("Error: Multiple decimal points\n");
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
                                    printf("Error: Division by zero\n");
                                    current_state = ERROR;
                                    break;
                                }
                                accumulator /= current_number;
                                break;
                            default:
                                break;
                        }
                    }
                    
                    switch (c) {
                        case '+': current_op = OP_ADD; break;
                        case '-': current_op = OP_SUBTRACT; break;
                        case '*': current_op = OP_MULTIPLY; break;
                        case '/': current_op = OP_DIVIDE; break;
                        default: break;
                    }
                    
                    current_number = 0.0;
                    has_decimal = 0;
                    decimal_place = 1.0;
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
                                    printf("Error: Division by zero\n");
                                    current_state = ERROR;
                                    break;
                                }
                                accumulator /= current_number;
                                break;
                            default:
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
                    printf("Error: Invalid character in number\n");
                    current_state = ERROR;
                }
                break;
                
            case READING_OPERATOR:
                if (isdigit(c)) {
                    current_number = c - '0';
                    current_state = READING_NUMBER;
                } else if (c == '+' || c == '-' || c == '*' || c == '/') {
                    printf("Error: Consecutive operators\n");
                    current_state = ERROR;
                } else if (c == '=') {
                    printf("Error: Operator before equals\n");
                    current_state = ERROR;
                } else if (c == 'q' || c == 'Q') {
                    current_state = DONE;
                } else if (!isspace(c)) {
                    printf("Error: Invalid character after operator\n");
                    current_state = ERROR;