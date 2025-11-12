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
    int count = 0;
    const char *ptr = input;
    
    while (*ptr && count <= max_degree) {
        double value = strtod(ptr, &endptr);
        if (ptr == endptr) {
            break;
        }
        if (value == 0.0 && *ptr != '0') {
            break;
        }
        if (isinf(value) || isnan(value)) {
            return -1;
        }
        
        *(coeffs + count) = value;
        count++;
        ptr = endptr;
        
        while (*ptr == ' ' || *ptr == ',') {
            ptr++;
        }
    }
    
    return count;
}

int main() {
    double coefficients[ARRAY_SIZE];
    char input_buffer[1024];
    int degree;
    double x_value;
    
    printf("Enter polynomial coefficients (space or comma separated): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (strlen(input_buffer) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    degree = validate_coefficients(input_buffer, coefficients, ARRAY_SIZE - 1);
    if (degree <= 0) {
        fprintf(stderr, "Invalid coefficients\n");
        return 1;
    }
    
    printf("Enter x value: ");
    if (scanf("%lf", &x_value) != 1) {
        fprintf(stderr, "Invalid x value\n");
        return 1;
    }
    
    if (isinf(x_value) || isnan(x_value)) {
        fprintf(stderr, "Invalid x value\n");
        return 1;
    }
    
    double result = compute_polynomial(coefficients, degree - 1, x_value);
    
    if (isinf(result) || isnan(result)) {
        fprintf(stderr, "Numerical overflow in computation\n");
        return 1;
    }
    
    printf("Polynomial value at x = %.6f: %.6f\n", x_value, result);
    
    return 0;
}