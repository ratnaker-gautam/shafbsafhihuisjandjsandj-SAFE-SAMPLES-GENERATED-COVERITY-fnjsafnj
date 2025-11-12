//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>
#include <float.h>

#define MAX_INPUT_LEN 256

double calculate_expression(const char* input) {
    double num1 = 0.0, num2 = 0.0;
    char op = 0;
    int parsed = sscanf(input, "%lf %c %lf", &num1, &op, &num2);
    
    if (parsed != 3) {
        return NAN;
    }
    
    switch (op) {
        case '+':
            if (fabs(num1) > DBL_MAX - fabs(num2)) return NAN;
            return num1 + num2;
        case '-':
            if (fabs(num1) > DBL_MAX - fabs(num2)) return NAN;
            return num1 - num2;
        case '*':
            if (fabs(num1) > DBL_MAX / fabs(num2)) return NAN;
            return num1 * num2;
        case '/':
            if (fabs(num2) < 1e-15) return NAN;
            return num1 / num2;
        case '^':
            if (num1 < 0 && fmod(num2, 1.0) != 0) return NAN;
            if (fabs(num1) > 1e100 || fabs(num2) > 1e100) return NAN;
            return pow(num1, num2);
        default:
            return NAN;
    }
}

int main() {
    char input[MAX_INPUT_LEN];
    int valid_count = 0;
    double total = 0.0;
    
    printf("Enter arithmetic expressions (e.g., 5 + 3, 2.5 * 4, 10 / 2)\n");
    printf("Enter 'quit' to exit\n");
    
    while (1) {
        printf("Expression %d: ", valid_count + 1);
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        
        if (strcmp(input, "quit") == 0) {
            break;
        }
        
        if (strlen(input) == 0) {
            continue;
        }
        
        double result = calculate_expression(input);
        
        if (!isnan(result)) {
            printf("Result: %.6f\n", result);
            total += result;
            valid_count++;
        } else {
            printf("Invalid expression or mathematical error\n");
        }
    }
    
    if (valid_count > 0) {
        printf("\nProcessed %d valid expressions\n", valid_count);
        printf("Sum of all results: %.6f\n", total);
        printf("Average result: %.6f\n", total / valid_count);
    } else {
        printf("No valid expressions processed\n");
    }
    
    return 0;
}