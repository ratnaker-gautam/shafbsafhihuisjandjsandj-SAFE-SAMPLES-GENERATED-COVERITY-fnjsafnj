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
    char input[1024];
    char *token;
    int count = 0;
    
    printf("Enter polynomial coefficients (space separated, highest degree first): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    token = strtok(input, " \t\n");
    while (token != NULL && count <= max_degree) {
        char *endptr;
        double value = strtod(token, &endptr);
        
        if (endptr == token || *endptr != '\0') {
            return -1;
        }
        
        if (!isfinite(value)) {
            return -1;
        }
        
        *(coeffs + count) = value;
        count++;
        token = strtok(NULL, " \t\n");
    }
    
    return count - 1;
}

int main(void) {
    double coefficients[MAX_SIZE];
    double derivative_coeffs[MAX_SIZE];
    int degree;
    double x;
    
    degree = read_coefficients(coefficients, MAX_SIZE - 1);
    if (degree < 0 || degree >= MAX_SIZE) {
        fprintf(stderr, "Error: Invalid coefficients or too many coefficients\n");
        return 1;
    }
    
    printf("Enter x value for evaluation: ");
    if (scanf("%lf", &x) != 1) {
        fprintf(stderr, "Error: Invalid x value\n");
        return 1;
    }
    
    if (!isfinite(x)) {
        fprintf(stderr, "Error: x value is not finite\n");
        return 1;
    }
    
    double poly_value = compute_polynomial(coefficients, degree, x);
    derivative(coefficients, degree, derivative_coeffs);
    double deriv_value = compute_polynomial(derivative_coeffs, degree - 1, x);
    
    printf("Polynomial value at x=%.3f: %.6f\n", x, poly_value);
    printf("Derivative value at x=%.3f: %.6f\n", x, deriv_value);
    
    return 0;
}