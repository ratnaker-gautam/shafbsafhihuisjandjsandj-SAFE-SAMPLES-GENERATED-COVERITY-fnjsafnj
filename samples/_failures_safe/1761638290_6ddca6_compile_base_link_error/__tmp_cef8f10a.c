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

void fill_fibonacci(int* arr, int count) {
    if (count < 1) return;
    
    int* ptr = arr;
    *ptr = 0;
    if (count == 1) return;
    
    ptr++;
    *ptr = 1;
    if (count == 2) return;
    
    for (int i = 2; i < count; i++) {
        *(arr + i) = *(arr + i - 1) + *(arr + i - 2);
    }
}

int main(void) {
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
    int fib_array[ARRAY_SIZE];
    fill_fibonacci(fib_array, 20);
    
    int* fib_ptr = fib_array;
    for (int i = 0; i < 20; i++) {
        printf("%d ", *fib_ptr);
        fib_ptr++;
        
        if ((i + 1) % 10 == 0) {
            printf("\n");
        }
    }
    
    printf("\nArray statistics:\n");
    int numbers[ARRAY_SIZE];
    int* num_ptr = numbers;
    
    for (int i = 0; i < ARRAY_SIZE; i++) {
        *num_ptr = i * 3 % 47;
        num_ptr++;
    }
    
    int sum = 0;
    int min_val = numbers[0];
    int max_val = numbers[0];
    
    num_ptr = numbers;
    for (int i = 0; i < ARRAY_SIZE; i++) {
        sum += *num_ptr;
        if (*num_ptr < min_val) min_val = *num_ptr;
        if (*num_ptr > max_val) max_val = *num_ptr;
        num_ptr++;
    }
    
    double average = (double)sum / ARRAY_SIZE;
    printf("Sum: %d, Average: %.2f\n", sum, average);
    printf("Min: %d, Max: %d\n", min_val, max_val);
    
    double variance = 0.0;
    num_ptr = numbers;
    for (int i = 0; i < ARRAY_SIZE; i++) {
        double diff = *num_ptr - average;
        variance += diff * diff;
        num_ptr++;
    }
    variance /= ARRAY_SIZE;
    
    printf("Variance: %.4f, Std Dev: %.4f\n", variance, sqrt(variance));
    
    return 0;
}