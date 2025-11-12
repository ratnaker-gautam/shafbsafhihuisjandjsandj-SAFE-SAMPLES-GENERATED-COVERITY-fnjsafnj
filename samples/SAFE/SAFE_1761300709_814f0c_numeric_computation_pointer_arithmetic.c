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
    int numbers[ARRAY_SIZE];
    double coefficients[5] = {2.0, -3.0, 1.5, -0.5, 0.1};
    
    printf("Polynomial coefficients: ");
    double* coeff_ptr = coefficients;
    for (int i = 0; i < 5; i++) {
        printf("%.2f ", *coeff_ptr);
        coeff_ptr++;
    }
    printf("\n");
    
    printf("Enter x value for polynomial evaluation: ");
    char input[100];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    double x;
    char* endptr;
    x = strtod(input, &endptr);
    if (endptr == input || (*endptr != '\n' && *endptr != '\0')) {
        printf("Invalid number format\n");
        return 1;
    }
    
    double result = compute_polynomial(coefficients, 4, x);
    printf("Polynomial result for x=%.2f: %.6f\n", x, result);
    
    printf("Enter number of Fibonacci numbers to generate (1-%d): ", ARRAY_SIZE);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    int count;
    if (!validate_integer_input(input, &count)) {
        printf("Invalid integer input\n");
        return 1;
    }
    
    if (count < 1 || count > ARRAY_SIZE) {
        printf("Count must be between 1 and %d\n", ARRAY_SIZE);
        return 1;
    }
    
    fill_fibonacci(numbers, count);
    
    printf("Fibonacci sequence: ");
    int* num_ptr = numbers;
    for (int i = 0; i < count; i++) {
        printf("%d ", *num_ptr);
        num_ptr++;
    }
    printf("\n");
    
    printf("Array statistics using pointer arithmetic:\n");
    int* start = numbers;
    int* end = numbers + count;
    int sum = 0;
    int min = *start;
    int max = *start;
    
    int* current = start;
    while (current < end) {
        sum += *current;
        if (*current < min) min = *current;
        if (*current > max) max = *current;
        current++;
    }
    
    double average = (double)sum / count;
    printf("Count: %d, Sum: %d, Average: %.2f\n", count, sum, average);
    printf("Min: %d, Max: %d\n", min, max);
    
    return 0;
}