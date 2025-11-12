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

void fill_fibonacci(int* arr, int count) {
    if (count < 1 || arr == NULL) return;
    
    int* ptr = arr;
    *ptr = 0;
    if (count == 1) return;
    
    ptr++;
    *ptr = 1;
    if (count == 2) return;
    
    for (int i = 2; i < count; i++) {
        if (i >= ARRAY_SIZE) break;
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
    double coefficients[5] = {2.0, -3.0, 1.5, 0.5, -1.0};
    int degree = 4;
    
    printf("Polynomial evaluation: f(x) = 2.0 - 3.0x + 1.5x^2 + 0.5x^3 - 1.0x^4\n");
    
    for (double x = -2.0; x <= 2.0; x += 0.5) {
        double result = compute_polynomial(coefficients, degree, x);
        printf("f(%.1f) = %.3f\n", x, result);
    }
    
    printf("\nFibonacci sequence generation:\n");
    int fib_count;
    char input[50];
    
    printf("Enter number of Fibonacci numbers to generate (1-50): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (!validate_integer_input(input, &fib_count)) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (fib_count < 1 || fib_count > 50) {
        printf("Count must be between 1 and 50\n");
        return 1;
    }
    
    int fibonacci[ARRAY_SIZE];
    fill_fibonacci(fibonacci, fib_count);
    
    printf("First %d Fibonacci numbers: ", fib_count);
    for (int i = 0; i < fib_count; i++) {
        printf("%d ", *(fibonacci + i));
    }
    printf("\n");
    
    printf("\nArray manipulation using pointer arithmetic:\n");
    int numbers[ARRAY_SIZE];
    int* start = numbers;
    int* end = numbers + ARRAY_SIZE - 1;
    
    for (int* ptr = start; ptr <= end; ptr++) {
        *ptr = (int)(ptr - start);
    }
    
    printf("Reversed array (first 10 elements): ");
    for (int* ptr = end; ptr >= start + ARRAY_SIZE - 10; ptr--) {
        printf("%d ", *ptr);
    }
    printf("\n");
    
    return 0;
}