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
    double current_value = 0.0;
    double accumulator = 0.0;
    char buffer[256];
    int buffer_pos = 0;
    int c;
    
    printf("Simple Calculator (enter '=' to calculate, 'q' to quit)\n");
    printf("Enter expression: ");
    
    while (state != DONE && state != ERROR) {
        c = getchar();
        
        if (c == EOF) {
            state = ERROR;
            break;
        }
        
        switch (state) {
            case INIT:
                if (isdigit(c) || c == '.') {
                    if (buffer_pos < 255) {
                        buffer[buffer_pos++] = (char)c;
                        state = READING_NUMBER;
                    } else {
                        state = ERROR;
                    }
                } else if (c == 'q' || c == 'Q') {
                    state = DONE;
                } else if (!isspace(c)) {
                    state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit(c) || c == '.') {
                    if (buffer_pos < 255) {
                        buffer[buffer_pos++] = (char)c;
                    } else {
                        state = ERROR;
                    }
                } else if (isspace(c)) {
                    buffer[buffer_pos] = '\0';
                    double num = strtod(buffer, NULL);
                    accumulator = num;
                    buffer_pos = 0;
                    state = READING_OPERATOR;
                } else if (c == '=') {
                    buffer[buffer_pos] = '\0';
                    double num = strtod(buffer, NULL);
                    accumulator = num;
                    state = CALCULATING;
                } else if (c == '+' || c == '-' || c == '*' || c == '/') {
                    buffer[buffer_pos] = '\0';
                    double num = strtod(buffer, NULL);
                    accumulator = num;
                    buffer_pos = 0;
                    
                    switch (c) {
                        case '+': current_op = OP_ADD; break;
                        case '-': current_op = OP_SUBTRACT; break;
                        case '*': current_op = OP_MULTIPLY; break;
                        case '/': current_op = OP_DIVIDE; break;
                    }
                    state = READING_NUMBER;
                } else if (c == 'q' || c == 'Q') {
                    state = DONE;
                } else {
                    state = ERROR;
                }
                break;
                
            case READING_OPERATOR:
                if (c == '+' || c == '-' || c == '*' || c == '/') {
                    switch (c) {
                        case '+': current_op = OP_ADD; break;
                        case '-': current_op = OP_SUBTRACT; break;
                        case '*': current_op = OP_MULTIPLY; break;
                        case '/': current_op = OP_DIVIDE; break;
                    }
                    state = READING_NUMBER;
                } else if (c == '=') {
                    state = CALCULATING;
                } else if (c == 'q' || c == 'Q') {
                    state = DONE;
                } else if (!isspace(c)) {
                    state = ERROR;
                }
                break;
                
            case CALCULATING:
                switch (current_op) {
                    case OP_ADD:
                        accumulator += current_value;
                        break;
                    case OP_SUBTRACT:
                        accumulator -= current_value;
                        break;
                    case OP_MULTIPLY:
                        accumulator *= current_value;
                        break;
                    case OP_DIVIDE:
                        if (current_value != 0.0) {
                            accumulator /= current_value;
                        } else {
                            state = ERROR;
                        }
                        break;
                    case OP_NONE:
                        break;
                }
                
                if (state != ERROR) {
                    printf("Result: %.2f\n", accumulator);
                    printf("Enter new expression or 'q' to quit: ");
                    state = INIT;
                    current_op = OP_NONE;
                    current_value = 0.0;
                    buffer_pos = 0;
                    
                    while ((c = getchar()) != '\n' && c != EOF) {
                        if (c == 'q' || c == 'Q') {
                            state = DONE;
                            break;
                        }
                    }
                }
                break;
                
            case ERROR:
                printf("Error: Invalid input\n");
                printf("Enter new expression or 'q' to quit: ");
                state = INIT;
                current_op = OP_NONE;
                current_value = 0.0;
                buffer_pos = 0;
                
                while ((c = getchar()) != '\n' && c != EOF) {
                    if (c == 'q' || c == 'Q') {
                        state = DONE;
                        break;
                    }
                }
                break;
                
            case DONE:
                break;
        }
    }
    
    printf("Goodbye!\n");
    return 0;
}