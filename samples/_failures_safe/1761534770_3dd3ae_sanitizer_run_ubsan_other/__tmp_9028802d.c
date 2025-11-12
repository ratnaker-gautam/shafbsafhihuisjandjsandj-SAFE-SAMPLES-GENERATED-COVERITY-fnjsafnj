//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ARRAY_SIZE 100

double compute_polynomial(double *coeffs, int degree, double x) {
    double result = 0.0;
    double *ptr = coeffs + degree;
    
    for (int i = degree; i >= 0; i--) {
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
    if (input == NULL) return 0;
    
    char *endptr;
    *result = strtod(input, &endptr);
    
    if (endptr == input) return 0;
    if (*endptr != '\0') return 0;
    
    return 1;
}

int main(void) {
    double coefficients[5] = {2.5, -1.3, 0.7, -0.2, 0.1};
    int fibonacci[ARRAY_SIZE];
    
    fill_fibonacci(fibonacci, ARRAY_SIZE);
    
    printf("Polynomial coefficients: ");
    double *coeff_ptr = coefficients;
    for (int i = 0; i < 5; i++) {
        printf("%.2f ", *coeff_ptr);
        coeff_ptr++;
    }
    printf("\n");
    
    printf("First 10 Fibonacci numbers: ");
    int *fib_ptr = fibonacci;
    for (int i = 0; i < 10; i++) {
        printf("%d ", *fib_ptr);
        fib_ptr++;
    }
    printf("\n");
    
    char input_buffer[256];
    printf("Enter x value for polynomial evaluation: ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    double x_value;
    if (!validate_input(input_buffer, &x_value)) {
        fprintf(stderr, "Invalid input. Please enter a valid number.\n");
        return 1;
    }
    
    double result = compute_polynomial(coefficients, 4, x_value);
    printf("Polynomial result for x=%.2f: %.6f\n", x_value, result);
    
    printf("Fibonacci numbers at indices 5, 10, 15: ");
    int *indices_ptr = fibonacci + 5;
    printf("%d ", *indices_ptr);
    indices_ptr = fibonacci + 10;
    printf("%d ", *indices_ptr);
    indices_ptr = fibonacci + 15;
    printf("%d\n", *indices_ptr);
    
    double sum = 0.0;
    double *coeff_sum_ptr = coefficients;
    for (int i = 0; i < 5; i++) {
        sum += *coeff_sum_ptr;
        coeff_sum_ptr++;
    }
    printf("Sum of coefficients: %.2f\n", sum);
    
    return 0;
}