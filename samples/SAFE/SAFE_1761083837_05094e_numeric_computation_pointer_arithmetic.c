//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
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
        if (i > 46) break;
        long long next = (long long)*(arr + i - 1) + (long long)*(arr + i - 2);
        if (next > INT_MAX) break;
        *(arr + i) = (int)next;
    }
}

int validate_integer(const char* str, int* result) {
    if (str == NULL || result == NULL || *str == '\0') return 0;
    
    const char* ptr = str;
    int sign = 1;
    
    if (*ptr == '-') {
        sign = -1;
        ptr++;
        if (*ptr == '\0') return 0;
    }
    
    long long value = 0;
    while (*ptr != '\0') {
        if (*ptr < '0' || *ptr > '9') return 0;
        
        value = value * 10 + (*ptr - '0');
        if (sign == 1 && value > INT_MAX) return 0;
        if (sign == -1 && value > (long long)INT_MAX + 1) return 0;
        ptr++;
    }
    
    if (sign == -1) value = -value;
    if (value < INT_MIN || value > INT_MAX) return 0;
    
    *result = (int)value;
    return 1;
}

int main() {
    double coefficients[5] = {2.0, -3.0, 1.5, 0.5, -1.0};
    int degree = 4;
    
    printf("Polynomial evaluation:\n");
    printf("Coefficients: ");
    double* coeff_ptr = coefficients;
    for (int i = 0; i <= degree; i++) {
        printf("%.2f ", *coeff_ptr);
        coeff_ptr++;
    }
    printf("\n");
    
    for (double x = -2.0; x <= 2.0; x += 0.5) {
        double result = compute_polynomial(coefficients, degree, x);
        printf("f(%.1f) = %.4f\n", x, result);
    }
    
    printf("\nFibonacci sequence:\n");
    int fib_array[ARRAY_SIZE] = {0};
    fill_fibonacci(fib_array, ARRAY_SIZE);
    
    int* fib_ptr = fib_array;
    for (int i = 0; i < 20; i++) {
        printf("%d ", *fib_ptr);
        fib_ptr++;
    }
    printf("\n");
    
    printf("\nInteger validation test:\n");
    char test_inputs[5][20] = {"123", "-456", "9999999999", "12a34", ""};
    
    for (int i = 0; i < 5; i++) {
        int validated_value;
        if (validate_integer(test_inputs[i], &validated_value)) {
            printf("'%s' -> valid: %d\n", test_inputs[i], validated_value);
        } else {
            printf("'%s' -> invalid\n", test_inputs[i]);
        }
    }
    
    printf("\nArray statistics:\n");
    int numbers[10] = {5, 2, 8, 1, 9, 3, 7, 4, 6, 0};
    int* num_ptr = numbers;
    int sum = 0;
    int min = *num_ptr;
    int max = *num_ptr;
    
    for (int i = 0; i < 10; i++) {
        sum += *num_ptr;
        if (*num_ptr < min) min = *num_ptr;
        if (*num_ptr > max) max = *num_ptr;
        num_ptr++;
    }
    
    printf("Sum: %d, Min: %d, Max: %d\n", sum, min, max);
    
    return 0;
}