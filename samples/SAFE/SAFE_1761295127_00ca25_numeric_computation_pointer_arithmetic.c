//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define ARRAY_SIZE 100

double compute_polynomial(double *coeffs, int degree, double x) {
    if (degree < 0 || coeffs == NULL) return 0.0;
    
    double result = 0.0;
    double *ptr = coeffs + degree;
    
    for (int i = degree; i >= 0; i--) {
        result = result * x + *ptr;
        ptr--;
    }
    
    return result;
}

void generate_coefficients(double *arr, int size) {
    if (arr == NULL || size <= 0) return;
    
    double *end = arr + size;
    for (double *ptr = arr; ptr < end; ptr++) {
        *ptr = (double)((ptr - arr) % 10) / 10.0;
    }
}

int validate_input(const char *input, double *result) {
    if (input == NULL || result == NULL) return 0;
    
    char *endptr;
    double val = strtod(input, &endptr);
    
    if (endptr == input || *endptr != '\0') return 0;
    if (val < -1000.0 || val > 1000.0) return 0;
    
    *result = val;
    return 1;
}

int main(void) {
    double coefficients[ARRAY_SIZE];
    double x_value;
    char input_buffer[256];
    
    generate_coefficients(coefficients, ARRAY_SIZE);
    
    printf("Polynomial coefficient array initialized with %d elements.\n", ARRAY_SIZE);
    printf("Enter x value for polynomial evaluation (-1000 to 1000): ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input.\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (!validate_input(input_buffer, &x_value)) {
        fprintf(stderr, "Invalid input. Please enter a valid number between -1000 and 1000.\n");
        return 1;
    }
    
    double result = compute_polynomial(coefficients, ARRAY_SIZE - 1, x_value);
    
    printf("Polynomial evaluation result: %.6f\n", result);
    
    double *ptr = coefficients;
    double sum = 0.0;
    double *end = coefficients + ARRAY_SIZE;
    
    while (ptr < end) {
        sum += *ptr;
        ptr++;
    }
    
    printf("Sum of all coefficients: %.6f\n", sum);
    
    return 0;
}