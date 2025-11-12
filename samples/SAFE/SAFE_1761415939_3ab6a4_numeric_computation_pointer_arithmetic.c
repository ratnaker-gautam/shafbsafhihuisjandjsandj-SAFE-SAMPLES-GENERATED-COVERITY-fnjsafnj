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
        result = result * x + *ptr;
        ptr--;
    }
    
    return result;
}

void fill_fibonacci(int* arr, int count) {
    if (arr == NULL || count <= 0 || count > ARRAY_SIZE) return;
    
    int* ptr = arr;
    
    if (count >= 1) *ptr = 0;
    if (count >= 2) *(ptr + 1) = 1;
    
    for (int i = 2; i < count; i++) {
        if (i > 0 && ptr + i - 1 >= arr && ptr + i - 2 >= arr) {
            long long sum = (long long)*(ptr + i - 1) + (long long)*(ptr + i - 2);
            if (sum > INT_MAX) sum = INT_MAX;
            *(ptr + i) = (int)sum;
        }
    }
}

int validate_integer(const char* str, int* value) {
    if (str == NULL || value == NULL) return 0;
    
    char* endptr;
    long num = strtol(str, &endptr, 10);
    
    if (endptr == str || *endptr != '\0') return 0;
    if (num < INT_MIN || num > INT_MAX) return 0;
    
    *value = (int)num;
    return 1;
}

int main() {
    int coefficients[ARRAY_SIZE];
    int degree;
    char input[256];
    
    printf("Enter polynomial degree (0-%d): ", ARRAY_SIZE - 1);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    if (!validate_integer(input, &degree)) {
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
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Input error\n");
            return 1;
        }
        
        if (!validate_integer(input, coefficients + i)) {
            printf("Invalid coefficient\n");
            return 1;
        }
    }
    
    double x_value;
    printf("Enter x value for evaluation: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    char* endptr;
    x_value = strtod(input, &endptr);
    if (endptr == input || *endptr != '\n' && *endptr != '\0') {
        printf("Invalid x value\n");
        return 1;
    }
    
    double result = compute_polynomial(coefficients, degree, x_value);
    printf("Polynomial value at x=%.2f: %.6f\n", x_value, result);
    
    int fib_count;
    printf("Enter Fibonacci sequence length (1-%d): ", ARRAY_SIZE);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    if (!validate_integer(input, &fib_count)) {
        printf("Invalid length\n");
        return 1;
    }
    
    if (fib_count <= 0 || fib_count > ARRAY_SIZE) {
        printf("Length out of range\n");
        return 1;
    }
    
    int fibonacci[ARRAY_SIZE];
    fill_fibonacci(fibonacci, fib_count);
    
    printf("Fibonacci sequence: ");
    for (int i = 0; i < fib_count; i++) {
        printf("%d ", fibonacci[i]);
    }
    printf("\n");
    
    return 0;
}