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
    
    if (val == HUGE_VAL || val == -HUGE_VAL || val != val) {
        return 0;
    }
    
    *result = val;
    return 1;
}

int main(void) {
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
    
    if (sscanf(input, "%d", &degree) != 1) {
        fprintf(stderr, "Invalid degree format\n");
        return 1;
    }
    
    if (degree < 1 || degree > 9) {
        fprintf(stderr, "Degree must be between 1 and 9\n");
        return 1;
    }
    
    printf("Enter %d coefficients (from constant term to highest degree):\n", degree + 1);
    
    for (int i = 0; i <= degree; i++) {
        printf("Coefficient %d: ", i);
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            fprintf(stderr, "Error reading input\n");
            return 1;
        }
        
        newline = strchr(input, '\n');
        if (newline) *newline = '\0';
        
        if (!safe_str_to_double(input, &coefficients[i])) {
            fprintf(stderr, "Invalid coefficient format\n");
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
    
    if (!safe_str_to_double(input, &x)) {
        fprintf(stderr, "Invalid x value format\n");
        return 1;
    }
    
    double result = compute_polynomial(x, coefficients, degree);
    
    printf("Polynomial: ");
    for (int i = 0; i <= degree; i++) {
        if (i > 0) printf(" + ");
        printf("%.2f", coefficients[i]);
        if (i > 0) printf("*x");
        if (i > 1) printf("^%d", i);
    }
    printf("\n");
    
    printf("Result at x = %.2f: %.6f\n", x, result);
    
    return 0;
}