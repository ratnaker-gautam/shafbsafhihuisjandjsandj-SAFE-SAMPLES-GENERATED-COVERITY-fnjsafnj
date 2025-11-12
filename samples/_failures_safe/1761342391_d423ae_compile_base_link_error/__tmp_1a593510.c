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
    
    if (count >= 1) *arr = 0;
    if (count >= 2) *(arr + 1) = 1;
    
    int* ptr = arr + 2;
    for (int i = 2; i < count; i++) {
        *ptr = *(ptr - 1) + *(ptr - 2);
        ptr++;
    }
}

double compute_statistics(int* data, int count, double* mean, double* stddev) {
    if (data == NULL || count <= 0 || mean == NULL || stddev == NULL) return 0.0;
    
    double sum = 0.0;
    double sum_sq = 0.0;
    int* ptr = data;
    
    for (int i = 0; i < count; i++) {
        sum += *ptr;
        sum_sq += (double)(*ptr) * (double)(*ptr);
        ptr++;
    }
    
    *mean = sum / count;
    *stddev = sqrt((sum_sq / count) - (*mean * *mean));
    
    return *stddev;
}

int main() {
    double coeffs[4] = {2.0, -3.0, 1.5, 4.0};
    double x = 2.5;
    double poly_result = compute_polynomial(coeffs, 3, x);
    printf("Polynomial at x=%.2f: %.4f\n", x, poly_result);
    
    int fib_sequence[ARRAY_SIZE];
    fill_fibonacci(fib_sequence, 20);
    
    printf("First 20 Fibonacci numbers: ");
    int* fib_ptr = fib_sequence;
    for (int i = 0; i < 20; i++) {
        printf("%d ", *fib_ptr);
        fib_ptr++;
    }
    printf("\n");
    
    int sample_data[10] = {12, 15, 18, 22, 25, 28, 30, 32, 35, 40};
    double mean, stddev;
    compute_statistics(sample_data, 10, &mean, &stddev);
    
    printf("Statistics - Mean: %.2f, StdDev: %.4f\n", mean, stddev);
    
    int matrix[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    int trace = 0;
    int* matrix_ptr = (int*)matrix;
    
    for (int i = 0; i < 3; i++) {
        trace += *(matrix_ptr + i * 3 + i);
    }
    
    printf("Matrix trace: %d\n", trace);
    
    return 0;
}