//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State { START, READING_NUM, READING_OP, FINISHED, ERROR };

int is_valid_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

int perform_operation(int a, int b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': 
            if (b != 0) return a / b;
            else return 0;
        default: return 0;
    }
}

int main(void) {
    char input[256];
    int result = 0;
    int current_num = 0;
    char current_op = '+';
    enum State state = START;
    int i = 0;
    int sign = 1;
    
    printf("Enter simple arithmetic expression (e.g., 5+3-2): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    while (i < (int)len && state != FINISHED && state != ERROR) {
        char c = input[i];
        
        switch (state) {
            case START:
                if (isdigit(c)) {
                    current_num = c - '0';
                    state = READING_NUM;
                } else if (c == '+' || c == '-') {
                    sign = (c == '+') ? 1 : -1;
                    state = READING_NUM;
                } else if (c == ' ') {
                } else {
                    state = ERROR;
                }
                break;
                
            case READING_NUM:
                if (isdigit(c)) {
                    if (current_num > 214748364 || (current_num == 214748364 && c > '7')) {
                        state = ERROR;
                        break;
                    }
                    current_num = current_num * 10 + (c - '0');
                } else if (is_valid_operator(c)) {
                    result = perform_operation(result, sign * current_num, current_op);
                    current_op = c;
                    current_num = 0;
                    sign = 1;
                    state = READING_OP;
                } else if (c == ' ') {
                    state = READING_NUM;
                } else if (c == '\0') {
                    result = perform_operation(result, sign * current_num, current_op);
                    state = FINISHED;
                } else {
                    state = ERROR;
                }
                break;
                
            case READING_OP:
                if (isdigit(c)) {
                    current_num = c - '0';
                    state = READING_NUM;
                } else if (c == '+' || c == '-') {
                    sign = (c == '+') ? 1 : -1;
                    state = READING_NUM;
                } else if (c == ' ') {
                } else {
                    state = ERROR;
                }
                break;
                
            default:
                break;
        }
        i++;
    }
    
    if (state == READING_NUM) {
        result = perform_operation(result, sign * current_num, current_op);
        state = FINISHED;
    }
    
    if (state == FINISHED) {
        printf("Result: %d\n", result);
    } else {
        printf("Invalid expression\n");
        return 1;
    }
    
    return 0;
}