//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ARRAY_SIZE 100

double compute_polynomial(double *coeffs, int degree, double x) {
    double result = 0.0;
    double *ptr = coeffs + degree;
    
    while (ptr >= coeffs) {
        result = result * x + *ptr;
        ptr--;
    }
    
    return result;
}

int validate_coefficients(double *coeffs, int degree) {
    if (degree < 0 || degree >= ARRAY_SIZE) {
        return 0;
    }
    
    double *start = coeffs;
    double *end = coeffs + degree;
    
    while (start <= end) {
        if (isnan(*start) || isinf(*start)) {
            return 0;
        }
        start++;
    }
    
    return 1;
}

int main(void) {
    double coefficients[ARRAY_SIZE];
    int degree;
    double x_value;
    
    printf("Enter polynomial degree (0-%d): ", ARRAY_SIZE - 1);
    if (scanf("%d", &degree) != 1) {
        fprintf(stderr, "Error: Invalid degree input\n");
        return 1;
    }
    
    if (degree < 0 || degree >= ARRAY_SIZE) {
        fprintf(stderr, "Error: Degree out of valid range\n");
        return 1;
    }
    
    printf("Enter %d coefficients (from constant term to highest degree):\n", degree + 1);
    for (int i = 0; i <= degree; i++) {
        if (scanf("%lf", &coefficients[i]) != 1) {
            fprintf(stderr, "Error: Invalid coefficient input\n");
            return 1;
        }
    }
    
    if (!validate_coefficients(coefficients, degree)) {
        fprintf(stderr, "Error: Invalid coefficients detected\n");
        return 1;
    }
    
    printf("Enter x value for evaluation: ");
    if (scanf("%lf", &x_value) != 1) {
        fprintf(stderr, "Error: Invalid x value input\n");
        return 1;
    }
    
    if (isnan(x_value) || isinf(x_value)) {
        fprintf(stderr, "Error: Invalid x value\n");
        return 1;
    }
    
    double result = compute_polynomial(coefficients, degree, x_value);
    
    printf("Polynomial evaluation result: %.6f\n", result);
    
    double *derivative_coeffs = coefficients + 1;
    if (degree > 0) {
        double derivative_result = 0.0;
        double *ptr = derivative_coeffs + (degree - 1);
        int current_degree = 1;
        
        while (ptr >= derivative_coeffs) {
            derivative_result = derivative_result * x_value + (*ptr * current_degree);
            ptr--;
            current_degree++;
        }
        
        printf("Derivative evaluation result: %.6f\n", derivative_result);
    }
    
    return 0;
}