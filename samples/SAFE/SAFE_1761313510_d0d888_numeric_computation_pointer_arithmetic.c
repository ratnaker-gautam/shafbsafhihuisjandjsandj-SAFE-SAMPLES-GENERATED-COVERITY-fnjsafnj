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

void fill_coefficients(double *arr, int size) {
    double *end = arr + size;
    double *current = arr;
    int counter = 0;
    
    while (current < end) {
        *current = (double)(counter % 5 + 1) / 10.0;
        current++;
        counter++;
    }
}

int validate_input(const char *input, double *result) {
    char *endptr;
    *result = strtod(input, &endptr);
    
    if (endptr == input || *endptr != '\n') {
        return 0;
    }
    
    if (*result == HUGE_VAL || *result == -HUGE_VAL) {
        return 0;
    }
    
    return 1;
}

int main(void) {
    double coefficients[ARRAY_SIZE];
    double x_value;
    char input_buffer[256];
    int degree;
    
    fill_coefficients(coefficients, ARRAY_SIZE);
    
    printf("Enter polynomial degree (1-%d): ", ARRAY_SIZE - 1);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *endptr;
    long temp_degree = strtol(input_buffer, &endptr, 10);
    
    if (endptr == input_buffer || *endptr != '\n') {
        fprintf(stderr, "Invalid degree format\n");
        return 1;
    }
    
    if (temp_degree < 1 || temp_degree >= ARRAY_SIZE) {
        fprintf(stderr, "Degree out of range\n");
        return 1;
    }
    
    degree = (int)temp_degree;
    
    printf("Enter x value: ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (!validate_input(input_buffer, &x_value)) {
        fprintf(stderr, "Invalid x value\n");
        return 1;
    }
    
    double result = compute_polynomial(coefficients, degree, x_value);
    
    printf("Polynomial value at x=%.3f: %.6f\n", x_value, result);
    
    double *coeff_ptr = coefficients;
    printf("First %d coefficients: ", degree + 1);
    for (int i = 0; i <= degree; i++) {
        printf("%.3f ", *coeff_ptr);
        coeff_ptr++;
    }
    printf("\n");
    
    return 0;
}