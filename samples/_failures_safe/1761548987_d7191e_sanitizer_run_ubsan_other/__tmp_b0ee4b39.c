//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ARRAY_SIZE 100

double compute_polynomial(double* coeffs, int degree, double x) {
    double result = 0.0;
    double* ptr = coeffs + degree;
    
    for (int i = degree; i >= 0; i--) {
        result = result * x + *ptr;
        ptr--;
    }
    
    return result;
}

void fill_fibonacci(int* arr, int size) {
    if (size < 1) return;
    
    int* ptr = arr;
    *ptr = 0;
    if (size == 1) return;
    
    ptr++;
    *ptr = 1;
    if (size == 2) return;
    
    for (int i = 2; i < size; i++) {
        *(arr + i) = *(arr + i - 1) + *(arr + i - 2);
    }
}

int validate_input(const char* input, int* value) {
    if (input == NULL) return 0;
    
    char* endptr;
    long temp = strtol(input, &endptr, 10);
    
    if (endptr == input || *endptr != '\0') return 0;
    if (temp < 1 || temp > 1000) return 0;
    
    *value = (int)temp;
    return 1;
}

int main() {
    double coefficients[5] = {2.0, -3.0, 1.5, -0.5, 0.1};
    int fibonacci_sequence[ARRAY_SIZE];
    
    fill_fibonacci(fibonacci_sequence, ARRAY_SIZE);
    
    printf("Polynomial coefficients: ");
    double* coeff_ptr = coefficients;
    for (int i = 0; i < 5; i++) {
        printf("%.2f ", *coeff_ptr);
        coeff_ptr++;
    }
    printf("\n");
    
    char input_buffer[64];
    printf("Enter x value for polynomial evaluation: ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input_buffer[strcspn(input_buffer, "\n")] = '\0';
    
    double x_value;
    char* endptr;
    x_value = strtod(input_buffer, &endptr);
    
    if (endptr == input_buffer || *endptr != '\0') {
        fprintf(stderr, "Invalid number format\n");
        return 1;
    }
    
    double result = compute_polynomial(coefficients, 4, x_value);
    printf("Polynomial result at x=%.2f: %.6f\n", x_value, result);
    
    printf("Enter Fibonacci sequence length (1-1000): ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input_buffer[strcspn(input_buffer, "\n")] = '\0';
    
    int fib_length;
    if (!validate_input(input_buffer, &fib_length)) {
        fprintf(stderr, "Invalid length. Must be integer 1-1000\n");
        return 1;
    }
    
    if (fib_length > ARRAY_SIZE) {
        fprintf(stderr, "Length exceeds maximum of %d\n", ARRAY_SIZE);
        return 1;
    }
    
    printf("First %d Fibonacci numbers:\n", fib_length);
    int* fib_ptr = fibonacci_sequence;
    for (int i = 0; i < fib_length; i++) {
        printf("%d ", *fib_ptr);
        fib_ptr++;
        
        if ((i + 1) % 10 == 0) printf("\n");
    }
    if (fib_length % 10 != 0) printf("\n");
    
    return 0;
}