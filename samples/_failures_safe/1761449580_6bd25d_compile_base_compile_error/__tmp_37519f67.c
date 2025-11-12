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

int main(void) {
    double coefficients[5] = {2.5, -1.8, 3.2, -0.5, 1.0};
    int degree = 4;
    
    printf("Polynomial evaluation: f(x) = 2.5 - 1.8x + 3.2x^2 - 0.5x^3 + 1.0x^4\n");
    
    for (double x = -2.0; x <= 2.0; x += 0.5) {
        double result = compute_polynomial(coefficients, degree, x);
        printf("f(%.1f) = %.3f\n", x, result);
    }
    
    printf("\nFibonacci sequence generation:\n");
    int fib_sequence[ARRAY_SIZE];
    fill_fibonacci(fib_sequence, 20);
    
    int* fib_ptr = fib_sequence;
    for (int i = 0; i < 20; i++) {
        printf("%d ", *fib_ptr);
        fib_ptr++;
    }
    printf("\n");
    
    printf("\nInteger validation test:\n");
    char test_inputs[5][20] = {"123", "-456", "789abc", "999999999999", "42"};
    
    for (int i = 0; i < 5; i++) {
        int validated_value;
        if (validate_integer_input(test_inputs[i], &validated_value)) {
            printf("'%s' -> valid: %d\n", test_inputs[i], validated_value);
        } else {
            printf("'%s' -> invalid\n", test_inputs[i]);
        }
    }
    
    printf("\nArray manipulation with pointer arithmetic:\n");
    double numbers[ARRAY_SIZE];
    double* num_ptr = numbers;
    
    for (int i = 0; i < ARRAY_SIZE; i++) {
        *num_ptr = sin(i * 0.1) * cos(i * 0.05);
        num_ptr++;
    }
    
    double sum = 0.0;
    double* sum_ptr = numbers;
    for (int i = 0; i < ARRAY_SIZE; i++) {
        sum += *sum_ptr;
        sum_ptr++;
    }
    
    printf("Sum of trigonometric values: %.6f\n", sum);
    printf("Average: %.6f\n", sum / ARRAY_SIZE);
    
    return 0;
}