//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define ARRAY_SIZE 100

double compute_polynomial(double* coeffs, int degree, double x) {
    if (degree < 0 || coeffs == NULL) return 0.0;
    
    double result = 0.0;
    double* ptr = coeffs + degree;
    
    for (int i = degree; i >= 0; i--) {
        if (i == degree) {
            result = *ptr;
        } else {
            result = result * x + *ptr;
        }
        ptr--;
    }
    
    return result;
}

void fill_fibonacci(int* arr, size_t size) {
    if (arr == NULL || size < 2) return;
    
    int* ptr = arr;
    *ptr = 0;
    ptr++;
    *ptr = 1;
    ptr++;
    
    for (size_t i = 2; i < size; i++) {
        if (ptr - arr >= (long)size) break;
        if (INT_MAX - *(ptr-1) < *(ptr-2)) break;
        *ptr = *(ptr-1) + *(ptr-2);
        ptr++;
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
        printf("Input error\n");
        return 1;
    }
    
    if (!validate_integer_input(input_buffer, &degree)) {
        printf("Invalid degree\n");
        return 1;
    }
    
    if (degree < 0 || degree >= ARRAY_SIZE) {
        printf("Degree out of range\n");
        return 1;
    }
    
    printf("Enter %d coefficients:\n", degree + 1);
    for (int i = 0; i <= degree; i++) {
        printf("Coefficient %d: ", i);
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            printf("Input error\n");
            return 1;
        }
        
        if (!validate_integer_input(input_buffer, &coefficients[i])) {
            printf("Invalid coefficient\n");
            return 1;
        }
    }
    
    printf("Enter x value: ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    double x;
    char* endptr;
    x = strtod(input_buffer, &endptr);
    if (endptr == input_buffer || *endptr != '\n') {
        printf("Invalid x value\n");
        return 1;
    }
    
    double result = compute_polynomial((double*)coefficients, degree, x);
    printf("Polynomial result: %.6f\n", result);
    
    int fibonacci[ARRAY_SIZE];
    fill_fibonacci(fibonacci, ARRAY_SIZE);
    
    printf("First %d Fibonacci numbers:\n", ARRAY_SIZE);
    int* fib_ptr = fibonacci;
    for (size_t i = 0; i < ARRAY_SIZE; i++) {
        printf("%d ", *fib_ptr);
        fib_ptr++;
        if ((i + 1) % 10 == 0) printf("\n");
    }
    
    return 0;
}