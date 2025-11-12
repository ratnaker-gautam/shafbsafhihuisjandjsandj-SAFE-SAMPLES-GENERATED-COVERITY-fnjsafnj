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
        *current = (counter % 5) + 1.0;
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
    
    if (*result < -1000.0 || *result > 1000.0) {
        return 0;
    }
    
    return 1;
}

int main(void) {
    double coefficients[ARRAY_SIZE];
    double x_value;
    char input_buffer[256];
    
    fill_coefficients(coefficients, ARRAY_SIZE);
    
    printf("Enter x value for polynomial evaluation (-1000 to 1000): ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (!validate_input(input_buffer, &x_value)) {
        fprintf(stderr, "Invalid input. Please enter a valid number between -1000 and 1000\n");
        return 1;
    }
    
    double result = compute_polynomial(coefficients, ARRAY_SIZE - 1, x_value);
    
    printf("Polynomial result: %.6f\n", result);
    
    return 0;
}