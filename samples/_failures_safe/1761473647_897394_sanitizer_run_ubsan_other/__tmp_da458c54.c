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

void fill_fibonacci(int *arr, int count) {
    if (count < 1) return;
    
    int *ptr = arr;
    *ptr = 0;
    if (count == 1) return;
    
    ptr++;
    *ptr = 1;
    if (count == 2) return;
    
    for (int i = 2; i < count; i++) {
        *(arr + i) = *(arr + i - 1) + *(arr + i - 2);
    }
}

int validate_input(const char *input, double *result) {
    if (input == NULL || result == NULL) return 0;
    
    char *endptr;
    *result = strtod(input, &endptr);
    
    if (endptr == input) return 0;
    if (*endptr != '\0' && *endptr != '\n') return 0;
    
    return 1;
}

int main(void) {
    double coefficients[ARRAY_SIZE];
    int fibonacci[ARRAY_SIZE];
    
    for (int i = 0; i < ARRAY_SIZE; i++) {
        coefficients[i] = (double)(i % 10) / 10.0;
    }
    
    fill_fibonacci(fibonacci, ARRAY_SIZE);
    
    printf("Enter a value for x to evaluate polynomial: ");
    char input_buffer[256];
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    double x_value;
    if (!validate_input(input_buffer, &x_value)) {
        fprintf(stderr, "Invalid input. Please enter a valid number.\n");
        return 1;
    }
    
    double poly_result = compute_polynomial(coefficients, 5, x_value);
    
    printf("Polynomial result for x=%.2f: %.6f\n", x_value, poly_result);
    
    printf("First 20 Fibonacci numbers: ");
    for (int i = 0; i < 20 && i < ARRAY_SIZE; i++) {
        printf("%d ", fibonacci[i]);
    }
    printf("\n");
    
    double *coeff_ptr = coefficients;
    double sum = 0.0;
    for (int i = 0; i < ARRAY_SIZE; i++) {
        sum += *(coeff_ptr + i);
    }
    
    printf("Sum of all coefficients: %.6f\n", sum);
    
    int *fib_ptr = fibonacci + ARRAY_SIZE - 1;
    printf("Last 5 Fibonacci numbers: ");
    for (int i = 0; i < 5 && fib_ptr >= fibonacci; i++) {
        printf("%d ", *fib_ptr);
        fib_ptr--;
    }
    printf("\n");
    
    return 0;
}