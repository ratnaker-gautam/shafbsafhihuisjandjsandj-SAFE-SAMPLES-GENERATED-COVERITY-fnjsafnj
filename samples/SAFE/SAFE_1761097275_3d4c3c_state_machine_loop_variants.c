//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: state_machine
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
                if (b < 0 && a < INT_MAX / b) return 0;
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
    printf("Enter simple arithmetic (e.g., 5+3): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
        len--;
    }
    
    if (len == 0 || len >= sizeof(input)) {
        printf("Invalid input\n");
        return 1;
    }
    
    enum State state = START;
    int num1 = 0, num2 = 0;
    char operator = '\0';
    int temp_num = 0;
    int sign = 1;
    size_t i = 0;
    
    while (i < len) {
        char c = input[i];
        
        switch (state) {
            case START:
                if (isspace((unsigned char)c)) {
                    i++;
                    continue;
                } else if (c == '-' || c == '+') {
                    sign = (c == '-') ? -1 : 1;
                    state = READING_NUM;
                    i++;
                } else if (isdigit((unsigned char)c)) {
                    temp_num = c - '0';
                    state = READING_NUM;
                    i++;
                } else {
                    state = ERROR;
                }
                break;
                
            case READING_NUM:
                if (isdigit((unsigned char)c)) {
                    if (temp_num > (INT_MAX - (c - '0')) / 10) {
                        state = ERROR;
                        break;
                    }
                    temp_num = temp_num * 10 + (c - '0');
                    i++;
                } else if (isspace((unsigned char)c)) {
                    i++;
                } else if (is_valid_operator(c)) {
                    num1 = temp_num * sign;
                    temp_num = 0;
                    sign = 1;
                    operator = c;
                    state = READING_OP;
                    i++;
                } else {
                    state = ERROR;
                }
                break;
                
            case READING_OP:
                if (isspace((unsigned char)c)) {
                    i++;
                } else if (c == '-' || c == '+') {
                    sign = (c == '-') ? -1 : 1;
                    state = READING_NUM;
                    i++;
                } else if (isdigit((unsigned char)c)) {
                    temp_num = c - '0';
                    state = READING_NUM;
                    i++;
                } else {
                    state = ERROR;
                }
                break;
                
            default:
                state = ERROR;
                break;
        }
        
        if (state == ERROR) break;
    }
    
    if (state == READING_NUM) {
        num2 = temp_num * sign;
        state = FINISHED;
    }
    
    if (state != FINISHED) {
        printf("Invalid expression\n");
        return 1;
    }
    
    int result = perform_operation(num1, num2, operator);
    printf("%d %c %d = %d\n", num1, operator, num2, result);
    
    return 0;
}