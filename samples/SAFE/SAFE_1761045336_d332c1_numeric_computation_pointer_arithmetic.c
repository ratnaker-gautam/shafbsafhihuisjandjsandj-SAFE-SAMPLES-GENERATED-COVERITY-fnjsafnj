//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_SIZE 100

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
    if (degree < 0 || degree >= MAX_SIZE) {
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
    double coefficients[MAX_SIZE];
    int degree;
    double x_value;
    
    printf("Enter polynomial degree (0-%d): ", MAX_SIZE - 1);
    if (scanf("%d", &degree) != 1) {
        printf("Invalid input for degree\n");
        return 1;
    }
    
    if (degree < 0 || degree >= MAX_SIZE) {
        printf("Degree out of valid range\n");
        return 1;
    }
    
    printf("Enter %d coefficients (from constant term to highest degree):\n", degree + 1);
    double *coeff_ptr = coefficients;
    int coeff_count = degree + 1;
    
    while (coeff_count > 0) {
        if (scanf("%lf", coeff_ptr) != 1) {
            printf("Invalid coefficient input\n");
            return 1;
        }
        coeff_ptr++;
        coeff_count--;
    }
    
    if (!validate_coefficients(coefficients, degree)) {
        printf("Invalid coefficients detected\n");
        return 1;
    }
    
    printf("Enter x value for evaluation: ");
    if (scanf("%lf", &x_value) != 1) {
        printf("Invalid input for x value\n");
        return 1;
    }
    
    if (isnan(x_value) || isinf(x_value)) {
        printf("Invalid x value\n");
        return 1;
    }
    
    double result = compute_polynomial(coefficients, degree, x_value);
    
    printf("Polynomial value at x = %.3f: %.6f\n", x_value, result);
    
    double *deriv_coeffs = coefficients + 1;
    int deriv_degree = degree - 1;
    
    if (deriv_degree >= 0) {
        double deriv_result = 0.0;
        double *deriv_ptr = deriv_coeffs + deriv_degree;
        int current_degree = 1;
        
        while (deriv_ptr >= deriv_coeffs) {
            deriv_result = deriv_result * x_value + (*deriv_ptr * current_degree);
            deriv_ptr--;
            current_degree++;
        }
        
        printf("Derivative value at x = %.3f: %.6f\n", x_value, deriv_result);
    }
    
    return 0;
}