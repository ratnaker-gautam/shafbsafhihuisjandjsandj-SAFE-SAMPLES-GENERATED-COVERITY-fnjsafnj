//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_LEN 256

double compute_polynomial(double x, double coeffs[], int degree) {
    double result = 0.0;
    double term = 1.0;
    
    for (int i = 0; i <= degree; i++) {
        result += coeffs[i] * term;
        term *= x;
    }
    
    return result;
}

int safe_str_to_double(const char *str, double *result) {
    char *endptr;
    double val = strtod(str, &endptr);
    
    if (endptr == str || *endptr != '\0') {
        return 0;
    }
    
    if (val == HUGE_VAL || val == -HUGE_VAL) {
        return 0;
    }
    
    *result = val;
    return 1;
}

int main() {
    char input[MAX_INPUT_LEN];
    double x;
    double coefficients[10];
    int degree;
    
    printf("Polynomial Evaluator\n");
    printf("Enter the degree of polynomial (1-9): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *newline = strchr(input, '\n');
    if (newline) *newline = '\0';
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    char *endptr;
    long deg = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\0' || deg < 1 || deg > 9) {
        fprintf(stderr, "Invalid degree. Must be integer between 1-9\n");
        return 1;
    }
    degree = (int)deg;
    
    printf("Enter %d coefficients (from constant term to highest degree):\n", degree + 1);
    for (int i = 0; i <= degree; i++) {
        printf("Coefficient %d: ", i);
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            fprintf(stderr, "Error reading input\n");
            return 1;
        }
        
        newline = strchr(input, '\n');
        if (newline) *newline = '\0';
        
        if (strlen(input) == 0) {
            fprintf(stderr, "Empty input\n");
            return 1;
        }
        
        if (!safe_str_to_double(input, &coefficients[i])) {
            fprintf(stderr, "Invalid coefficient\n");
            return 1;
        }
    }
    
    printf("Enter x value to evaluate: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    newline = strchr(input, '\n');
    if (newline) *newline = '\0';
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    if (!safe_str_to_double(input, &x)) {
        fprintf(stderr, "Invalid x value\n");
        return 1;
    }
    
    double result = compute_polynomial(x, coefficients, degree);
    
    printf("P(%g) = ", x);
    for (int i = 0; i <= degree; i++) {
        if (i == 0) {
            printf("%g", coefficients[i]);
        } else if (i == 1) {
            printf(" + %g*x", coefficients[i]);
        } else {
            printf(" + %g*x^%d", coefficients[i], i);
        }
    }
    printf("\n");
    printf("Result: %g\n", result);
    
    return 0;
}