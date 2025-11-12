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

int validate_input(const char *input, double *array, int max_size) {
    char *endptr;
    int count = 0;
    const char *ptr = input;
    
    while (*ptr != '\0' && count < max_size) {
        while (*ptr == ' ' || *ptr == '\t') ptr++;
        
        if (*ptr == '\0') break;
        
        double value = strtod(ptr, &endptr);
        if (ptr == endptr) return -1;
        
        if (value == HUGE_VAL || value == -HUGE_VAL) return -1;
        
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
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    int num_coeffs = validate_input(input_buffer, coefficients, MAX_SIZE);
    if (num_coeffs <= 0) {
        fprintf(stderr, "Invalid coefficients input\n");
        return 1;
    }
    
    degree = num_coeffs - 1;
    
    printf("Enter x value for polynomial evaluation: ");
    if (scanf("%lf", &x_value) != 1) {
        fprintf(stderr, "Invalid x value\n");
        return 1;
    }
    
    if (x_value == HUGE_VAL || x_value == -HUGE_VAL) {
        fprintf(stderr, "x value out of range\n");
        return 1;
    }
    
    result = compute_polynomial(coefficients, degree, x_value);
    
    if (result == HUGE_VAL || result == -HUGE_VAL) {
        fprintf(stderr, "Result out of range\n");
        return 1;
    }
    
    printf("Polynomial degree: %d\n", degree);
    printf("P(%.6f) = %.6f\n", x_value, result);
    
    return 0;
}