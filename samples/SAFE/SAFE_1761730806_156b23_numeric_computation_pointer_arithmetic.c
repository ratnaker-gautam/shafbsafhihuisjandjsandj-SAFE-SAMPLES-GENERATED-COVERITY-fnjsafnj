//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ARRAY_SIZE 100

double compute_polynomial(double* coeffs, int degree, double x) {
    double result = 0.0;
    double* ptr = coeffs + degree;
    
    for (int i = degree; i >= 0; i--) {
        result = result * x + *ptr;
        ptr--;
    }
    
    return result;
}

int validate_coefficients(double* coeffs, int degree) {
    if (degree < 0 || degree >= ARRAY_SIZE) {
        return 0;
    }
    
    double* start = coeffs;
    double* end = coeffs + degree;
    
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
    
    printf("Enter %d coefficients:\n", degree + 1);
    double* coeff_ptr = coefficients;
    for (int i = 0; i <= degree; i++) {
        if (scanf("%lf", coeff_ptr) != 1) {
            printf("Invalid coefficient input\n");
            return 1;
        }
        coeff_ptr++;
    }
    
    if (!validate_coefficients(coefficients, degree)) {
        printf("Invalid coefficients detected\n");
        return 1;
    }
    
    printf("Enter x value: ");
    if (scanf("%lf", &x_value) != 1) {
        printf("Invalid x value\n");
        return 1;
    }
    
    if (isnan(x_value) || isinf(x_value)) {
        printf("Invalid x value\n");
        return 1;
    }
    
    double result = compute_polynomial(coefficients, degree, x_value);
    
    printf("Polynomial coefficients: ");
    double* print_ptr = coefficients;
    for (int i = 0; i <= degree; i++) {
        printf("%.2f ", *print_ptr);
        print_ptr++;
    }
    printf("\n");
    
    printf("P(%.2f) = %.6f\n", x_value, result);
    
    return 0;
}