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

int validate_coefficients(const char *input, double *coeffs, int max_degree) {
    char *endptr;
    double *ptr = coeffs;
    int count = 0;
    
    while (*input && count < max_degree) {
        double value = strtod(input, &endptr);
        if (endptr == input) break;
        
        *ptr = value;
        ptr++;
        count++;
        input = endptr;
    }
    
    return count;
}

int main(void) {
    double coefficients[ARRAY_SIZE];
    double x_value;
    char input_buffer[1024];
    int degree;
    
    printf("Enter polynomial coefficients (space separated): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    degree = validate_coefficients(input_buffer, coefficients, ARRAY_SIZE - 1);
    if (degree < 1) {
        fprintf(stderr, "Invalid or no coefficients provided\n");
        return 1;
    }
    
    printf("Enter x value: ");
    if (scanf("%lf", &x_value) != 1) {
        fprintf(stderr, "Invalid x value\n");
        return 1;
    }
    
    if (!isfinite(x_value)) {
        fprintf(stderr, "x value must be finite\n");
        return 1;
    }
    
    double result = compute_polynomial(coefficients, degree - 1, x_value);
    
    printf("Polynomial coefficients: ");
    double *ptr = coefficients;
    for (int i = 0; i < degree; i++) {
        printf("%.2f ", *ptr);
        ptr++;
    }
    printf("\n");
    
    printf("P(%.2f) = %.6f\n", x_value, result);
    
    return 0;
}