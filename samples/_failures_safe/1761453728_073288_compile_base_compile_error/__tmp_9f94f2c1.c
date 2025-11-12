//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>
#include <float.h>

#define MAX_INPUT_LEN 256

double calculate_expression(const char* expr) {
    double num1 = 0.0, num2 = 0.0;
    char op = 0;
    int parsed = sscanf(expr, "%lf %c %lf", &num1, &op, &num2);
    
    if (parsed != 3) {
        return NAN;
    }
    
    switch (op) {
        case '+':
            if ((num2 > 0 && num1 > DBL_MAX - num2) || 
                (num2 < 0 && num1 < -DBL_MAX - num2)) {
                return NAN;
            }
            return num1 + num2;
        case '-':
            if ((num2 < 0 && num1 > DBL_MAX + num2) || 
                (num2 > 0 && num1 < -DBL_MAX + num2)) {
                return NAN;
            }
            return num1 - num2;
        case '*':
            if (num2 != 0.0 && fabs(num1) > DBL_MAX / fabs(num2)) {
                return NAN;
            }
            return num1 * num2;
        case '/':
            if (num2 == 0.0) {
                return NAN;
            }
            return num1 / num2;
        case '^':
            if (num1 == 0.0 && num2 < 0.0) {
                return NAN;
            }
            if (num1 < 0.0 && fmod(num2, 1.0) != 0.0) {
                return NAN;
            }
            if (num1 != 0.0 && fabs(log(fabs(num1)) * num2) > log(DBL_MAX)) {
                return NAN;
            }
            return pow(num1, num2);
        default:
            return NAN;
    }
}

int main() {
    char input[MAX_INPUT_LEN + 1];
    int valid_inputs = 0;
    double results[100];
    
    printf("Enter up to 100 arithmetic expressions (format: number operator number)\n");
    printf("Supported operators: + - * / ^\n");
    printf("Enter 'quit' to finish early\n\n");
    
    while (valid_inputs < 100) {
        printf("Expression %d: ", valid_inputs + 1);
        
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
        
        if (len >= MAX_INPUT_LEN) {
            printf("Input too long. Maximum %d characters.\n", MAX_INPUT_LEN);
            continue;
        }
        
        double result = calculate_expression(input);
        
        if (isnan(result)) {
            printf("Invalid expression or mathematical error.\n");
            continue;
        }
        
        results[valid_inputs] = result;
        printf("Result: %.6f\n\n", result);
        valid_inputs++;
    }
    
    if (valid_inputs > 0) {
        printf("\nSummary of %d calculations:\n", valid_inputs);
        double sum = 0.0;
        double min_val = results[0];
        double max_val = results[0];
        
        for (int i = 0; i < valid_inputs; i++) {
            printf("Result %d: %.6f\n", i + 1, results[i]);
            sum += results[i];
            if (results[i] < min_val) min_val = results[i];
            if (results[i] > max_val) max_val = results[i];
        }
        
        printf("\nStatistics:\n");
        printf("Average: %.6f\n", sum / valid_inputs);
        printf("Minimum: %.6f\n", min_val);
        printf("Maximum: %.6f\n", max_val);
    } else {
        printf("No valid calculations performed.\n");
    }
    
    return 0;
}