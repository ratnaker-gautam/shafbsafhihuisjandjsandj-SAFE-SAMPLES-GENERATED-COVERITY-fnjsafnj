//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define ARRAY_SIZE 100

double compute_polynomial(double *coeffs, int degree, double x) {
    if (coeffs == NULL || degree < 0) return 0.0;
    
    double result = 0.0;
    double *ptr = coeffs + degree;
    
    for (int i = degree; i >= 0; i--) {
        result = result * x + *ptr;
        ptr--;
    }
    
    return result;
}

void fill_fibonacci(int *arr, int count) {
    if (arr == NULL || count <= 0 || count > ARRAY_SIZE) return;
    
    if (count >= 1) *(arr + 0) = 0;
    if (count >= 2) *(arr + 1) = 1;
    
    int *ptr = arr + 2;
    for (int i = 2; i < count; i++) {
        *ptr = *(ptr - 1) + *(ptr - 2);
        ptr++;
    }
}

double calculate_mean(int *data, int size) {
    if (data == NULL || size <= 0) return 0.0;
    
    long long sum = 0;
    int *ptr = data;
    
    for (int i = 0; i < size; i++) {
        if (*ptr > 0 && sum > LLONG_MAX - *ptr) return 0.0;
        if (*ptr < 0 && sum < LLONG_MIN - *ptr) return 0.0;
        sum += *ptr;
        ptr++;
    }
    
    return (double)sum / size;
}

int main() {
    double coefficients[5] = {2.5, -1.3, 0.8, -0.2, 1.0};
    int degree = 4;
    double x_value = 2.5;
    
    double poly_result = compute_polynomial(coefficients, degree, x_value);
    printf("Polynomial evaluation at x=%.2f: %.6f\n", x_value, poly_result);
    
    int fibonacci[ARRAY_SIZE];
    int fib_count = 20;
    
    fill_fibonacci(fibonacci, fib_count);
    
    printf("First %d Fibonacci numbers:\n", fib_count);
    int *fib_ptr = fibonacci;
    for (int i = 0; i < fib_count; i++) {
        printf("%d ", *fib_ptr);
        fib_ptr++;
        if ((i + 1) % 10 == 0) printf("\n");
    }
    printf("\n");
    
    int sample_data[10] = {15, 22, 8, 19, 31, 12, 25, 18, 29, 14};
    double mean_value = calculate_mean(sample_data, 10);
    printf("Mean of sample data: %.2f\n", mean_value);
    
    int matrix[3][4] = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}};
    printf("Matrix elements using pointer arithmetic:\n");
    int *matrix_ptr = &matrix[0][0];
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%3d ", *matrix_ptr);
            matrix_ptr++;
        }
        printf("\n");
    }
    
    return 0;
}