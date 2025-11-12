//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

enum State { START, READING_NUM, READING_OP, FINISHED, ERROR };

int is_valid_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

int perform_operation(int a, int b, char op) {
    switch (op) {
        case '+': 
            if ((b > 0 && a > INT_MAX - b) || (b < 0 && a < INT_MIN - b)) return 0;
            return a + b;
        case '-': 
            if ((b < 0 && a > INT_MAX + b) || (b > 0 && a < INT_MIN + b)) return 0;
            return a - b;
        case '*': 
            if (a > 0) {
                if (b > 0 && a > INT_MAX / b) return 0;
                if (b < 0 && a < INT_MAX / b) return 0;
            } else if (a < 0) {
                if (b > 0 && a < INT_MIN / b) return 0;
                if (b < 0 && a > INT_MAX / b) return 0;
            }
            return a * b;
        case '/': 
            if (b == 0) return 0;
            if (a == INT_MIN && b == -1) return 0;
            return a / b;
        default: return 0;
    }
}

int main(void) {
    char input[256];
    int result = 0;
    int current_num = 0;
    char current_op = '+';
    enum State state = START;
    int has_number = 0;
    int i = 0;
    
    printf("Enter simple arithmetic expression (e.g., 5+3*2): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
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
                    has_number = 1;
                    state = READING_NUM;
                } else if (c == ' ') {
                } else {
                    state = ERROR;
                }
                break;
                
            case READING_NUM:
                if (isdigit(c)) {
                    if (current_num <= (INT_MAX - (c - '0')) / 10) {
                        current_num = current_num * 10 + (c - '0');
                    } else {
                        state = ERROR;
                    }
                } else if (is_valid_operator(c)) {
                    result = perform_operation(result, current_num, current_op);
                    current_op = c;
                    has_number = 0;
                    state = READING_OP;
                } else if (c == ' ') {
                    state = READING_NUM;
                } else if (c == '\0') {
                    result = perform_operation(result, current_num, current_op);
                    state = FINISHED;
                } else {
                    state = ERROR;
                }
                break;
                
            case READING_OP:
                if (isdigit(c)) {
                    current_num = c - '0';
                    has_number = 1;
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
    
    if (state == READING_NUM && has_number) {
        result = perform_operation(result, current_num, current_op);
        state = FINISHED;
    }
    
    if (state == FINISHED) {
        printf("Result: %d\n", result);
    } else {
        printf("Error: Invalid expression\n");
        return 1;
    }
    
    return 0;
}