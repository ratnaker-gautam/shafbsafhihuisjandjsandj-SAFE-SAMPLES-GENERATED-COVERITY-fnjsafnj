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
        case '+': 
            if ((b > 0 && a > INT_MAX - b) || (b < 0 && a < INT_MIN - b)) return 0;
            return a + b;
        case '-': 
            if ((b < 0 && a > INT_MAX + b) || (b > 0 && a < INT_MIN + b)) return 0;
            return a - b;
        case '*': 
            if (a > 0 && b > 0 && a > INT_MAX / b) return 0;
            if (a > 0 && b < 0 && b < INT_MIN / a) return 0;
            if (a < 0 && b > 0 && a < INT_MIN / b) return 0;
            if (a < 0 && b < 0 && a < INT_MAX / b) return 0;
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
    int numbers[10];
    char operators[10];
    int num_count = 0;
    int op_count = 0;
    enum State current_state = START;
    int current_num = 0;
    int has_digit = 0;
    
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
    
    int i = 0;
    while (i < len && current_state != ERROR && current_state != DONE) {
        char c = input[i];
        
        switch (current_state) {
            case START:
                if (isdigit(c)) {
                    current_num = c - '0';
                    has_digit = 1;
                    current_state = READING_NUM;
                } else if (isspace(c)) {
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUM:
                if (isdigit(c)) {
                    if (current_num <= (INT_MAX - (c - '0')) / 10) {
                        current_num = current_num * 10 + (c - '0');
                    } else {
                        current_state = ERROR;
                    }
                } else if (is_valid_operator(c)) {
                    if (num_count < 10) {
                        numbers[num_count++] = current_num;
                        current_num = 0;
                        has_digit = 0;
                    }
                    if (op_count < 10) {
                        operators[op_count++] = c;
                    }
                    current_state = READING_OP;
                } else if (c == '\0' || i == len - 1) {
                    if (num_count < 10) {
                        numbers[num_count++] = current_num;
                    }
                    current_state = DONE;
                } else if (isspace(c)) {
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_OP:
                if (isdigit(c)) {
                    current_num = c - '0';
                    has_digit = 1;
                    current_state = READING_NUM;
                } else if (isspace(c)) {
                } else {
                    current_state = ERROR;
                }
                break;
                
            default:
                break;
        }
        i++;
    }
    
    if (current_state == READING_NUM && has_digit) {
        if (num_count < 10) {
            numbers[num_count++] = current_num;
        }
        current_state = DONE;
    }
    
    if (current_state == ERROR || num_count == 0 || num_count != op_count + 1) {
        printf("Invalid expression format\n");
        return 1;
    }
    
    int result = numbers[0];
    for (int j = 0; j < op_count; j++) {
        int temp = perform_operation(result, numbers[j + 1], operators[j]);
        if (temp == 0 && numbers[j + 1] != 0 && operators[j] != '*') {
            printf("Arithmetic overflow detected\n");
            return 1;
        }
        result = temp;
    }
    
    printf("Result: %d\n", result);
    
    return 0;
}