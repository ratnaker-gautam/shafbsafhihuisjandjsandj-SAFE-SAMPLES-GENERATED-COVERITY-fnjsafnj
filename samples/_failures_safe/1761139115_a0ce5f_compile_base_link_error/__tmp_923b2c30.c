//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_LEN 256

double calculate_expression(const char* input) {
    double num1 = 0.0, num2 = 0.0;
    char op = 0;
    int parsed = sscanf(input, "%lf %c %lf", &num1, &op, &num2);
    
    if (parsed != 3) {
        return NAN;
    }
    
    if (op != '+' && op != '-' && op != '*' && op != '/' && op != '^') {
        return NAN;
    }
    
    switch (op) {
        case '+':
            return num1 + num2;
        case '-':
            return num1 - num2;
        case '*':
            return num1 * num2;
        case '/':
            if (fabs(num2) < 1e-15) {
                return NAN;
            }
            return num1 / num2;
        case '^':
            if (num1 < 0 && fmod(num2, 1.0) != 0) {
                return NAN;
            }
            if (num1 == 0 && num2 < 0) {
                return NAN;
            }
            return pow(num1, num2);
        default:
            return NAN;
    }
}

int main() {
    char input[MAX_INPUT_LEN + 1];
    int line_count = 0;
    
    printf("Enter number of calculations (1-100): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (sscanf(input, "%d", &line_count) != 1) {
        fprintf(stderr, "Invalid number format\n");
        return 1;
    }
    
    if (line_count < 1 || line_count > 100) {
        fprintf(stderr, "Number must be between 1 and 100\n");
        return 1;
    }
    
    printf("Enter %d expressions (format: number operator number):\n", line_count);
    
    for (int i = 0; i < line_count; i++) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            fprintf(stderr, "Error reading expression\n");
            return 1;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        
        if (len >= MAX_INPUT_LEN) {
            fprintf(stderr, "Input too long\n");
            return 1;
        }
        
        double result = calculate_expression(input);
        
        if (isnan(result)) {
            printf("Expression %d: Invalid input or operation\n", i + 1);
        } else if (isinf(result)) {
            printf("Expression %d: Result overflow\n", i + 1);
        } else {
            printf("Expression %d: %.6f\n", i + 1, result);
        }
    }
    
    return 0;
}