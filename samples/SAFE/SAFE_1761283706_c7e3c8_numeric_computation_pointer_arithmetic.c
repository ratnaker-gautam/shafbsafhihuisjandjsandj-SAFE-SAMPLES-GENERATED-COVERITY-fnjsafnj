//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_SIZE 100

double compute_polynomial(double *coeffs, int degree, double x) {
    double result = 0.0;
    double *ptr = coeffs + degree;
    
    for (int i = degree; i >= 0; i--) {
        result = result * x + *ptr;
        ptr--;
    }
    return result;
}

void derivative(double *coeffs, int degree, double *result) {
    if (degree < 1) {
        *result = 0.0;
        return;
    }
    
    double *src = coeffs + 1;
    double *dst = result;
    
    for (int i = 1; i <= degree; i++) {
        *dst = *src * i;
        src++;
        dst++;
    }
}

int read_coefficients(double *coeffs, int max_degree) {
    char buffer[256];
    int degree;
    
    printf("Enter polynomial degree (0-%d): ", max_degree);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return -1;
    }
    
    if (sscanf(buffer, "%d", &degree) != 1) {
        return -1;
    }
    
    if (degree < 0 || degree > max_degree) {
        return -1;
    }
    
    printf("Enter %d coefficients (from constant term to highest degree):\n", degree + 1);
    for (int i = 0; i <= degree; i++) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        if (sscanf(buffer, "%lf", coeffs + i) != 1) {
            return -1;
        }
    }
    
    return degree;
}

int main(void) {
    double coeffs[MAX_SIZE];
    double deriv_coeffs[MAX_SIZE];
    double x, value, deriv_value;
    int degree;
    
    degree = read_coefficients(coeffs, MAX_SIZE - 1);
    if (degree < 0) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    printf("Enter x value to evaluate: ");
    if (scanf("%lf", &x) != 1) {
        fprintf(stderr, "Error: Invalid x value\n");
        return 1;
    }
    
    value = compute_polynomial(coeffs, degree, x);
    printf("P(%.2f) = %.6f\n", x, value);
    
    if (degree > 0) {
        derivative(coeffs, degree, deriv_coeffs);
        deriv_value = compute_polynomial(deriv_coeffs, degree - 1, x);
        printf("P'(%.2f) = %.6f\n", x, deriv_value);
    } else {
        printf("P'(%.2f) = 0.000000\n", x);
    }
    
    printf("Polynomial coefficients: ");
    double *ptr = coeffs;
    for (int i = 0; i <= degree; i++) {
        printf("%.2f ", *ptr);
        ptr++;
    }
    printf("\n");
    
    if (degree > 0) {
        printf("Derivative coefficients: ");
        double *dptr = deriv_coeffs;
        for (int i = 0; i < degree; i++) {
            printf("%.2f ", *dptr);
            dptr++;
        }
        printf("\n");
    }
    
    return 0;
}