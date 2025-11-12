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
            if (b == 0) return 0;
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
    int i = 0;
    int sign = 1;
    
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
                if (isspace(c)) {
                    i++;
                } else if (c == '-' || c == '+') {
                    sign = (c == '-') ? -1 : 1;
                    state = READING_NUM;
                    i++;
                } else if (isdigit(c)) {
                    sign = 1;
                    state = READING_NUM;
                } else {
                    state = ERROR;
                }
                break;
                
            case READING_NUM:
                if (isdigit(c)) {
                    current_num = current_num * 10 + (c - '0');
                    i++;
                } else if (is_valid_operator(c)) {
                    result = perform_operation(result, sign * current_num, current_op);
                    current_op = c;
                    current_num = 0;
                    sign = 1;
                    state = READING_OP;
                    i++;
                } else if (c == '\0' || isspace(c)) {
                    result = perform_operation(result, sign * current_num, current_op);
                    state = FINISHED;
                } else {
                    state = ERROR;
                }
                break;
                
            case READING_OP:
                if (isspace(c)) {
                    i++;
                } else if (c == '-' || c == '+') {
                    sign = (c == '-') ? -1 : 1;
                    state = READING_NUM;
                    i++;
                } else if (isdigit(c)) {
                    sign = 1;
                    state = READING_NUM;
                } else {
                    state = ERROR;
                }
                break;
                
            default:
                state = ERROR;
                break;
        }
    }
    
    if (state == FINISHED) {
        printf("Result: %d\n", result);
    } else if (state == READING_NUM) {
        result = perform_operation(result, sign * current_num, current_op);
        printf("Result: %d\n", result);
    } else {
        printf("Error: Invalid expression\n");
        return 1;
    }
    
    return 0;
}