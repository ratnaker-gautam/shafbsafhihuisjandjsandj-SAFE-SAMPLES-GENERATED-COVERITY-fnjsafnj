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
    printf("Enter simple arithmetic expression (e.g., 5+3): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    enum State current_state = START;
    int operand1 = 0, operand2 = 0;
    char operator = '\0';
    int temp_value = 0;
    int sign = 1;
    size_t i = 0;
    
    while (i < len) {
        char c = input[i];
        
        switch (current_state) {
            case START:
                if (isspace((unsigned char)c)) {
                    i++;
                    continue;
                } else if (c == '-' || c == '+') {
                    sign = (c == '-') ? -1 : 1;
                    current_state = READING_NUM;
                    i++;
                } else if (isdigit((unsigned char)c)) {
                    temp_value = c - '0';
                    current_state = READING_NUM;
                    i++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUM:
                if (isdigit((unsigned char)c)) {
                    temp_value = temp_value * 10 + (c - '0');
                    i++;
                } else if (is_valid_operator(c)) {
                    operand1 = sign * temp_value;
                    operator = c;
                    temp_value = 0;
                    sign = 1;
                    current_state = READING_OP;
                    i++;
                } else if (isspace((unsigned char)c)) {
                    i++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_OP:
                if (isspace((unsigned char)c)) {
                    i++;
                    continue;
                } else if (c == '-' || c == '+') {
                    sign = (c == '-') ? -1 : 1;
                    i++;
                } else if (isdigit((unsigned char)c)) {
                    temp_value = c - '0';
                    i++;
                    while (i < len && isdigit((unsigned char)input[i])) {
                        temp_value = temp_value * 10 + (input[i] - '0');
                        i++;
                    }
                    operand2 = sign * temp_value;
                    current_state = FINISHED;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case FINISHED:
                i = len;
                break;
                
            case ERROR:
                printf("Invalid expression format\n");
                return 1;
        }
    }
    
    if (current_state != FINISHED) {
        printf("Incomplete expression\n");
        return 1;
    }
    
    int result = perform_operation(operand1, operand2, operator);
    printf("%d %c %d = %d\n", operand1, operator, operand2, result);
    
    return 0;
}