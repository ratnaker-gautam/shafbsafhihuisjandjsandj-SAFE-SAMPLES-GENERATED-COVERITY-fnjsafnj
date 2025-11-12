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
    int i = 0;
    
    while (current < end) {
        *current = (double)(i % 5 + 1) / 10.0;
        current++;
        i++;
    }
}

int read_integer(const char *prompt, int min, int max) {
    char buffer[32];
    char *endptr;
    long value;
    
    printf("%s", prompt);
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return -1;
    }
    
    value = strtol(buffer, &endptr, 10);
    
    if (endptr == buffer || *endptr != '\n') {
        return -1;
    }
    
    if (value < min || value > max) {
        return -1;
    }
    
    return (int)value;
}

double read_double(const char *prompt) {
    char buffer[32];
    char *endptr;
    double value;
    
    printf("%s", prompt);
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return NAN;
    }
    
    value = strtod(buffer, &endptr);
    
    if (endptr == buffer || *endptr != '\n') {
        return NAN;
    }
    
    return value;
}

int main(void) {
    double coefficients[ARRAY_SIZE];
    int degree;
    double x_value;
    double result;
    
    fill_coefficients(coefficients, ARRAY_SIZE);
    
    degree = read_integer("Enter polynomial degree (0-99): ", 0, ARRAY_SIZE - 1);
    if (degree == -1) {
        fprintf(stderr, "Invalid degree input\n");
        return 1;
    }
    
    x_value = read_double("Enter x value: ");
    if (isnan(x_value)) {
        fprintf(stderr, "Invalid x value input\n");
        return 1;
    }
    
    result = compute_polynomial(coefficients, degree, x_value);
    
    printf("Polynomial evaluation result: %.6f\n", result);
    
    double *coeff_ptr = coefficients;
    double *end_ptr = coefficients + degree;
    
    printf("Coefficients used: ");
    while (coeff_ptr <= end_ptr) {
        printf("%.2f ", *coeff_ptr);
        coeff_ptr++;
    }
    printf("\n");
    
    return 0;
}