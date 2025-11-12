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
    if (temp < 1 || temp > 100) return 0;
    
    *value = (int)temp;
    return 1;
}

int main() {
    char buffer[256];
    int n;
    
    printf("Enter number of Fibonacci terms to generate (1-100): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    } else if (len == sizeof(buffer) - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    if (!validate_input(buffer, &n)) {
        fprintf(stderr, "Invalid input. Please enter integer 1-100\n");
        return 1;
    }
    
    int fib_array[ARRAY_SIZE];
    fill_fibonacci(fib_array, n);
    
    printf("First %d Fibonacci numbers:\n", n);
    for (int i = 0; i < n; i++) {
        printf("%d ", *(fib_array + i));
    }
    printf("\n");
    
    double poly_coeffs[] = {2.0, -3.0, 1.0, 5.0};
    int degree = 3;
    
    printf("\nPolynomial coefficients: ");
    for (int i = 0; i <= degree; i++) {
        printf("%.1f ", *(poly_coeffs + i));
    }
    printf("\n");
    
    printf("Enter x value for polynomial evaluation: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char* endptr;
    double x = strtod(buffer, &endptr);
    if (endptr == buffer || *endptr != '\n') {
        fprintf(stderr, "Invalid number format\n");
        return 1;
    }
    
    double result = compute_polynomial(poly_coeffs, degree, x);
    printf("Polynomial result at x=%.2f: %.2f\n", x, result);
    
    return 0;
}