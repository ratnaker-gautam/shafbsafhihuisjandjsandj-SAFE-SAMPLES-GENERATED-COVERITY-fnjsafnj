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
        printf("Invalid input for degree\n");
        return 1;
    }
    
    if (degree < 0 || degree >= ARRAY_SIZE) {
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
    
    printf("Enter x value to evaluate polynomial: ");
    if (scanf("%lf", &x_value) != 1) {
        printf("Invalid x value input\n");
        return 1;
    }
    
    if (isnan(x_value) || isinf(x_value)) {
        printf("Invalid x value\n");
        return 1;
    }
    
    double result = compute_polynomial(coefficients, degree, x_value);
    
    if (isnan(result) || isinf(result)) {
        printf("Numerical overflow/underflow in computation\n");
        return 1;
    }
    
    printf("Polynomial value at x = %.6f: %.6f\n", x_value, result);
    
    double *deriv_coeffs = coefficients + 1;
    int deriv_degree = degree - 1;
    
    if (deriv_degree >= 0) {
        double deriv_result = 0.0;
        double *ptr = deriv_coeffs + deriv_degree;
        int power = deriv_degree + 1;
        
        while (ptr >= deriv_coeffs) {
            deriv_result = deriv_result * x_value + (*ptr) * power;
            ptr--;
            power--;
        }
        
        if (!isnan(deriv_result) && !isinf(deriv_result)) {
            printf("Derivative value at x = %.6f: %.6f\n", x_value, deriv_result);
        }
    }
    
    return 0;
}