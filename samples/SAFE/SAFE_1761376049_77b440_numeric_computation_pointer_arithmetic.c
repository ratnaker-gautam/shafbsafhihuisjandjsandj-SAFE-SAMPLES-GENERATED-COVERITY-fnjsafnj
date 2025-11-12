//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ARRAY_SIZE 100

double compute_polynomial(double* coeffs, int degree, double x) {
    double result = 0.0;
    double* ptr = coeffs + degree;
    
    while (ptr >= coeffs) {
        result = result * x + *ptr;
        ptr--;
    }
    
    return result;
}

int validate_coefficients(double* coeffs, int degree) {
    if (degree < 0 || degree >= ARRAY_SIZE) {
        return 0;
    }
    
    double* end = coeffs + degree;
    while (coeffs <= end) {
        if (!isfinite(*coeffs)) {
            return 0;
        }
        coeffs++;
    }
    return 1;
}

int main(void) {
    double coefficients[ARRAY_SIZE];
    int polynomial_degree;
    double x_value;
    
    printf("Enter polynomial degree (0-%d): ", ARRAY_SIZE - 1);
    if (scanf("%d", &polynomial_degree) != 1) {
        printf("Invalid input for degree\n");
        return 1;
    }
    
    if (polynomial_degree < 0 || polynomial_degree >= ARRAY_SIZE) {
        printf("Degree out of valid range\n");
        return 1;
    }
    
    printf("Enter %d coefficients (from highest to lowest degree):\n", polynomial_degree + 1);
    for (int i = 0; i <= polynomial_degree; i++) {
        if (scanf("%lf", coefficients + i) != 1) {
            printf("Invalid coefficient input\n");
            return 1;
        }
    }
    
    if (!validate_coefficients(coefficients, polynomial_degree)) {
        printf("Invalid coefficients detected\n");
        return 1;
    }
    
    printf("Enter x value for evaluation: ");
    if (scanf("%lf", &x_value) != 1) {
        printf("Invalid x value input\n");
        return 1;
    }
    
    if (!isfinite(x_value)) {
        printf("Invalid x value\n");
        return 1;
    }
    
    double result = compute_polynomial(coefficients, polynomial_degree, x_value);
    
    if (!isfinite(result)) {
        printf("Result is not finite\n");
        return 1;
    }
    
    printf("Polynomial value at x = %.6f: %.6f\n", x_value, result);
    
    return 0;
}