//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
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

int validate_input(const char *input, double *array, int max_size) {
    char *endptr;
    int count = 0;
    const char *ptr = input;
    
    while (*ptr && count < max_size) {
        while (*ptr == ' ' || *ptr == '\t') ptr++;
        if (!*ptr) break;
        
        double value = strtod(ptr, &endptr);
        if (ptr == endptr) return -1;
        
        array[count] = value;
        count++;
        ptr = endptr;
    }
    
    return count;
}

int main(void) {
    double coefficients[MAX_SIZE];
    char input_buffer[1024];
    int degree;
    
    printf("Enter polynomial coefficients from highest to lowest degree: ");
    if (!fgets(input_buffer, sizeof(input_buffer), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    int num_coeffs = validate_input(input_buffer, coefficients, MAX_SIZE);
    if (num_coeffs <= 0) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    degree = num_coeffs - 1;
    
    printf("Enter x value: ");
    if (!fgets(input_buffer, sizeof(input_buffer), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *endptr;
    double x = strtod(input_buffer, &endptr);
    if (endptr == input_buffer) {
        fprintf(stderr, "Invalid x value\n");
        return 1;
    }
    
    double result = compute_polynomial(coefficients, degree, x);
    
    printf("Polynomial degree: %d\n", degree);
    printf("P(%.6f) = %.6f\n", x, result);
    
    double *deriv_coeffs = coefficients + 1;
    if (degree > 0) {
        double derivative = 0.0;
        double *ptr = deriv_coeffs + (degree - 1);
        int current_degree = degree;
        
        while (ptr >= deriv_coeffs) {
            derivative = derivative * x + (*ptr * current_degree);
            ptr--;
            current_degree--;
        }
        
        printf("P'(%.6f) = %.6f\n", x, derivative);
    }
    
    return 0;
}