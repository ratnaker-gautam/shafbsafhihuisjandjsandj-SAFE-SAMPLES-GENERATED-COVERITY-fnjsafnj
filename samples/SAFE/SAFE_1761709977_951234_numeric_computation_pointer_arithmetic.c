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

int validate_input(const char* input, int* result) {
    if (input == NULL || result == NULL) return 0;
    
    char* endptr;
    long val = strtol(input, &endptr, 10);
    
    if (endptr == input || *endptr != '\0') return 0;
    if (val < 1 || val > 100) return 0;
    
    *result = (int)val;
    return 1;
}

int main() {
    char input_buffer[256];
    int array_size;
    
    printf("Enter array size (1-100): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (!validate_input(input_buffer, &array_size)) {
        fprintf(stderr, "Invalid input. Please enter an integer between 1 and 100.\n");
        return 1;
    }
    
    int fibonacci_array[ARRAY_SIZE];
    fill_fibonacci(fibonacci_array, array_size);
    
    printf("Fibonacci sequence:\n");
    for (int i = 0; i < array_size; i++) {
        printf("%d ", *(fibonacci_array + i));
    }
    printf("\n");
    
    double poly_coeffs[] = {2.0, -3.0, 1.0, 4.0};
    int poly_degree = 3;
    
    printf("Polynomial coefficients: ");
    for (int i = 0; i <= poly_degree; i++) {
        printf("%.1f ", *(poly_coeffs + i));
    }
    printf("\n");
    
    printf("Polynomial values at x=0,1,2:\n");
    for (double x = 0.0; x <= 2.0; x += 1.0) {
        double result = compute_polynomial(poly_coeffs, poly_degree, x);
        printf("f(%.1f) = %.2f\n", x, result);
    }
    
    int* fib_ptr = fibonacci_array;
    int sum = 0;
    for (int i = 0; i < array_size; i++) {
        sum += *fib_ptr;
        fib_ptr++;
    }
    
    printf("Sum of first %d Fibonacci numbers: %d\n", array_size, sum);
    
    return 0;
}