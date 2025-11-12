//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    INIT,
    READING_NUMBER,
    READING_OPERATOR,
    FINISHED,
    ERROR
};

int main(void) {
    char input[256];
    double numbers[10];
    char operators[9];
    int num_count = 0;
    int op_count = 0;
    enum State current_state = INIT;
    double result = 0.0;
    int i = 0;
    
    printf("Enter a simple arithmetic expression (e.g., 5 + 3 * 2): ");
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
    
    while (i < len && current_state != ERROR && current_state != FINISHED) {
        char c = input[i];
        
        switch (current_state) {
            case INIT:
                if (isspace(c)) {
                    i++;
                } else if (isdigit(c) || c == '.' || c == '+' || c == '-') {
                    current_state = READING_NUMBER;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (num_count >= 10) {
                    current_state = ERROR;
                    break;
                }
                
                char *endptr;
                numbers[num_count] = strtod(&input[i], &endptr);
                if (endptr == &input[i]) {
                    current_state = ERROR;
                    break;
                }
                
                num_count++;
                i = endptr - input;
                current_state = READING_OPERATOR;
                break;
                
            case READING_OPERATOR:
                if (isspace(c)) {
                    i++;
                } else if (c == '+' || c == '-' || c == '*' || c == '/') {
                    if (op_count >= 9) {
                        current_state = ERROR;
                        break;
                    }
                    operators[op_count] = c;
                    op_count++;
                    i++;
                    current_state = READING_NUMBER;
                } else if (c == '\0') {
                    current_state = FINISHED;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case FINISHED:
            case ERROR:
                break;
        }
    }
    
    if (current_state == ERROR || num_count == 0) {
        printf("Invalid expression format\n");
        return 1;
    }
    
    if (num_count != op_count + 1) {
        printf("Mismatched numbers and operators\n");
        return 1;
    }
    
    result = numbers[0];
    for (int j = 0; j < op_count; j++) {
        switch (operators[j]) {
            case '+':
                result += numbers[j+1];
                break;
            case '-':
                result -= numbers[j+1];
                break;
            case '*':
                result *= numbers[j+1];
                break;
            case '/':
                if (numbers[j+1] == 0.0) {
                    printf("Division by zero\n");
                    return 1;
                }
                result /= numbers[j+1];
                break;
            default:
                printf("Invalid operator\n");
                return 1;
        }
    }
    
    printf("Result: %.2f\n", result);
    return 0;
}