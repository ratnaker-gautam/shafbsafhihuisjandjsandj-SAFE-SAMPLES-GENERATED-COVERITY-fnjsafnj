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
    double x_value, result;
    
    printf("Enter polynomial coefficients (space separated, highest degree first): ");
    
    if (!fgets(input_buffer, sizeof(input_buffer), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    int num_coeffs = validate_input(input_buffer, coefficients, MAX_SIZE);
    if (num_coeffs <= 0) {
        fprintf(stderr, "Invalid coefficients input\n");
        return 1;
    }
    
    degree = num_coeffs - 1;
    
    printf("Enter x value: ");
    if (scanf("%lf", &x_value) != 1) {
        fprintf(stderr, "Invalid x value\n");
        return 1;
    }
    
    if (!isfinite(x_value)) {
        fprintf(stderr, "x value must be finite\n");
        return 1;
    }
    
    result = compute_polynomial(coefficients, degree, x_value);
    
    if (!isfinite(result)) {
        fprintf(stderr, "Result is not finite\n");
        return 1;
    }
    
    printf("P(%.2f) = %.6f\n", x_value, result);
    
    printf("Polynomial: ");
    double *coeff_ptr = coefficients;
    for (int i = degree; i >= 0; i--) {
        if (i == degree) {
            printf("%.2fx^%d", *coeff_ptr, i);
        } else if (i > 1) {
            printf(" + %.2fx^%d", *coeff_ptr, i);
        } else if (i == 1) {
            printf(" + %.2fx", *coeff_ptr);
        } else {
            printf(" + %.2f", *coeff_ptr);
        }
        coeff_ptr++;
    }
    printf("\n");
    
    return 0;
}