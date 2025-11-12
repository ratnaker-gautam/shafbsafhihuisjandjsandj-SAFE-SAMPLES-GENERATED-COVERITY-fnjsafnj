//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: state_machine
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
    
    printf("Simple Calculator (Enter '=' to calculate, 'q' to quit)\n");
    printf("Enter expression: ");
    
    while (current_state != DONE && current_state != ERROR) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            current_state = ERROR;
            break;
        }
        
        pos = 0;
        while (current_state != DONE && current_state != ERROR && input[pos] != '\0') {
            ch = input[pos++];
            
            if (ch == '\n') continue;
            
            switch (current_state) {
                case INIT:
                    if (isdigit(ch)) {
                        current_number = ch - '0';
                        current_state = READING_NUMBER;
                    } else if (ch == 'q' || ch == 'Q') {
                        current_state = DONE;
                    } else if (isspace(ch)) {
                        continue;
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
                    } else if (isspace(ch)) {
                        current_state = READING_OPERATOR;
                    } else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
                        accumulator = current_number;
                        current_number = 0.0;
                        has_decimal = 0;
                        decimal_place = 1.0;
                        
                        switch (ch) {
                            case '+': current_op = OP_ADD; break;
                            case '-': current_op = OP_SUBTRACT; break;
                            case '*': current_op = OP_MULTIPLY; break;
                            case '/': current_op = OP_DIVIDE; break;
                        }
                        current_state = READING_NUMBER;
                    } else if (ch == '=') {
                        accumulator = current_number;
                        current_state = CALCULATING;
                    } else {
                        current_state = ERROR;
                    }
                    break;
                    
                case READING_OPERATOR:
                    if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
                        accumulator = current_number;
                        current_number = 0.0;
                        has_decimal = 0;
                        decimal_place = 1.0;
                        
                        switch (ch) {
                            case '+': current_op = OP_ADD; break;
                            case '-': current_op = OP_SUBTRACT; break;
                            case '*': current_op = OP_MULTIPLY; break;
                            case '/': current_op = OP_DIVIDE; break;
                        }
                        current_state = READING_NUMBER;
                    } else if (isspace(ch)) {
                        continue;
                    } else {
                        current_state = ERROR;
                    }
                    break;
                    
                case CALCULATING:
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
                        case OP_NONE:
                            accumulator = current_number;
                            break;
                    }
                    
                    if (current_state != ERROR) {
                        printf("Result: %.6f\n", accumulator);
                        current_number = accumulator;
                        current_op = OP_NONE;
                        current_state = INIT;
                        printf("Enter new expression (or 'q' to quit): ");
                    }
                    break;
                    
                case ERROR:
                    printf("Error: Invalid input\n");
                    current_state = INIT;
                    current_number = 0.0;
                    accumulator = 0.0;
                    current_op = OP_NONE;
                    has_decimal = 0;
                    decimal_place = 1.0;
                    printf("Enter expression: ");
                    break;
                    
                case DONE:
                    break;
            }
        }
    }
    
    printf("Goodbye!\n");
    return 0;
}