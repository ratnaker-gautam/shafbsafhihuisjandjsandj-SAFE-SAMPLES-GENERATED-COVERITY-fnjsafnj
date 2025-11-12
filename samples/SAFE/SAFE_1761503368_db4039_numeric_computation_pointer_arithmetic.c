//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

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

int validate_integer_input(const char* input, int* result) {
    if (input == NULL || result == NULL) return 0;
    
    char* endptr;
    long val = strtol(input, &endptr, 10);
    
    if (endptr == input || *endptr != '\0') return 0;
    if (val < INT_MIN || val > INT_MAX) return 0;
    
    *result = (int)val;
    return 1;
}

int main() {
    int coefficients[ARRAY_SIZE];
    int degree;
    char input_buffer[256];
    
    printf("Enter polynomial degree (0-%d): ", ARRAY_SIZE - 1);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    input_buffer[strcspn(input_buffer, "\n")] = '\0';
    
    if (!validate_integer_input(input_buffer, &degree)) {
        fprintf(stderr, "Invalid degree input\n");
        return 1;
    }
    
    if (degree < 0 || degree >= ARRAY_SIZE) {
        fprintf(stderr, "Degree out of range\n");
        return 1;
    }
    
    printf("Enter %d coefficients:\n", degree + 1);
    for (int i = 0; i <= degree; i++) {
        printf("Coefficient %d: ", i);
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            fprintf(stderr, "Input error\n");
            return 1;
        }
        
        input_buffer[strcspn(input_buffer, "\n")] = '\0';
        
        if (!validate_integer_input(input_buffer, coefficients + i)) {
            fprintf(stderr, "Invalid coefficient input\n");
            return 1;
        }
    }
    
    printf("Enter x value for polynomial evaluation: ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    input_buffer[strcspn(input_buffer, "\n")] = '\0';
    
    double x;
    char* endptr;
    x = strtod(input_buffer, &endptr);
    if (endptr == input_buffer || *endptr != '\0') {
        fprintf(stderr, "Invalid x value\n");
        return 1;
    }
    
    double result = compute_polynomial((double*)coefficients, degree, x);
    printf("Polynomial result: %.6f\n", result);
    
    int fib_count;
    printf("Enter Fibonacci sequence length (1-%d): ", ARRAY_SIZE);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    input_buffer[strcspn(input_buffer, "\n")] = '\0';
    
    if (!validate_integer_input(input_buffer, &fib_count)) {
        fprintf(stderr, "Invalid Fibonacci count\n");
        return 1;
    }
    
    if (fib_count < 1 || fib_count > ARRAY_SIZE) {
        fprintf(stderr, "Fibonacci count out of range\n");
        return 1;
    }
    
    int fibonacci[ARRAY_SIZE];
    fill_fibonacci(fibonacci, fib_count);
    
    printf("Fibonacci sequence: ");
    for (int i = 0; i < fib_count; i++) {
        printf("%d ", *(fibonacci + i));
    }
    printf("\n");
    
    return 0;
}