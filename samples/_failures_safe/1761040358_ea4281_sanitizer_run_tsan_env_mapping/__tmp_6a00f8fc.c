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

void derivative(double *coeffs, int degree, double *result) {
    if (degree < 1) {
        result[0] = 0.0;
        return;
    }
    
    double *src = coeffs + 1;
    double *dst = result;
    int new_degree = degree - 1;
    
    for (int i = 1; i <= degree; i++) {
        *dst = *src * i;
        src++;
        dst++;
    }
}

int read_coefficients(double *coeffs, int max_degree) {
    char input[256];
    char *token;
    int count = 0;
    
    printf("Enter polynomial coefficients from highest to lowest degree (space separated): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    input[strcspn(input, "\n")] = 0;
    
    token = strtok(input, " ");
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
        token = strtok(NULL, " ");
    }
    
    return count - 1;
}

int main(void) {
    double coefficients[MAX_SIZE];
    double derivative_coeffs[MAX_SIZE];
    int degree;
    double x;
    char input[256];
    char *endptr;
    
    degree = read_coefficients(coefficients, MAX_SIZE - 1);
    if (degree < 0) {
        fprintf(stderr, "Error: Invalid coefficients input\n");
        return 1;
    }
    
    printf("Enter x value for evaluation: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error: Failed to read x value\n");
        return 1;
    }
    
    x = strtod(input, &endptr);
    if (endptr == input || *endptr != '\n' && *endptr != '\0') {
        fprintf(stderr, "Error: Invalid x value\n");
        return 1;
    }
    
    if (!isfinite(x)) {
        fprintf(stderr, "Error: x value is not finite\n");
        return 1;
    }
    
    double result = compute_polynomial(coefficients, degree, x);
    printf("P(%.2f) = %.6f\n", x, result);
    
    derivative(coefficients, degree, derivative_coeffs);
    double deriv_result = compute_polynomial(derivative_coeffs, degree - 1, x);
    printf("P'(%.2f) = %.6f\n", x, deriv_result);
    
    return 0;
}