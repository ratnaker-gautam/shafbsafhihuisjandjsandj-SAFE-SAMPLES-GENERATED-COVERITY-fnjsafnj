//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

enum State { START, READING_NUM, READING_OP, ERROR, DONE };

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
    printf("Enter simple arithmetic expression (e.g., 5+3): ");
    
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
    
    enum State state = START;
    int current_num = 0;
    int first_num = 0;
    char operation = '\0';
    int result = 0;
    int i = 0;
    
    while (state != DONE && state != ERROR && i < (int)len) {
        char c = input[i];
        
        switch (state) {
            case START:
                if (isdigit(c)) {
                    current_num = c - '0';
                    state = READING_NUM;
                } else if (isspace(c)) {
                } else {
                    state = ERROR;
                }
                break;
                
            case READING_NUM:
                if (isdigit(c)) {
                    int digit = c - '0';
                    if (current_num > (INT_MAX - digit) / 10) {
                        state = ERROR;
                        break;
                    }
                    current_num = current_num * 10 + digit;
                } else if (is_valid_operator(c)) {
                    first_num = current_num;
                    operation = c;
                    current_num = 0;
                    state = READING_OP;
                } else if (isspace(c)) {
                } else {
                    state = ERROR;
                }
                break;
                
            case READING_OP:
                if (isdigit(c)) {
                    current_num = c - '0';
                    state = READING_NUM;
                } else if (isspace(c)) {
                } else {
                    state = ERROR;
                }
                break;
                
            default:
                state = ERROR;
                break;
        }
        i++;
    }
    
    if (state == READING_NUM && operation != '\0') {
        result = perform_operation(first_num, current_num, operation);
        state = DONE;
    } else if (state != DONE) {
        state = ERROR;
    }
    
    if (state == ERROR) {
        printf("Invalid expression format\n");
        return 1;
    }
    
    printf("%d %c %d = %d\n", first_num, operation, current_num, result);
    return 0;
}