//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: state_machine
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
    int has_number = 0;
    int sign = 1;
    
    enum State state = START;
    
    printf("Enter simple arithmetic expression (e.g., 5+3-2): ");
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
    
    for (size_t i = 0; i <= len; i++) {
        char c = input[i];
        
        switch (state) {
            case START:
                if (isdigit(c)) {
                    current_num = c - '0';
                    has_number = 1;
                    state = READING_NUM;
                } else if (c == '+' || c == '-') {
                    sign = (c == '-') ? -1 : 1;
                    state = READING_NUM;
                } else if (c == '\0' && has_number) {
                    state = FINISHED;
                } else if (c != ' ' && c != '\0') {
                    state = ERROR;
                }
                break;
                
            case READING_NUM:
                if (isdigit(c)) {
                    if (current_num > 214748364) {
                        state = ERROR;
                        break;
                    }
                    current_num = current_num * 10 + (c - '0');
                    if (current_num < 0) {
                        state = ERROR;
                        break;
                    }
                } else if (is_valid_operator(c) || c == '\0') {
                    current_num *= sign;
                    sign = 1;
                    result = perform_operation(result, current_num, current_op);
                    current_num = 0;
                    has_number = 1;
                    
                    if (c == '\0') {
                        state = FINISHED;
                    } else {
                        current_op = c;
                        state = READING_OP;
                    }
                } else if (c != ' ') {
                    state = ERROR;
                }
                break;
                
            case READING_OP:
                if (isdigit(c)) {
                    current_num = c - '0';
                    state = READING_NUM;
                } else if (c == '+' || c == '-') {
                    sign = (c == '-') ? -1 : 1;
                    state = READING_NUM;
                } else if (c != ' ') {
                    state = ERROR;
                }
                break;
                
            case FINISHED:
            case ERROR:
                break;
        }
        
        if (state == ERROR || state == FINISHED) {
            break;
        }
    }
    
    if (state == FINISHED && has_number) {
        printf("Result: %d\n", result);
    } else {
        printf("Invalid expression\n");
        return 1;
    }
    
    return 0;
}