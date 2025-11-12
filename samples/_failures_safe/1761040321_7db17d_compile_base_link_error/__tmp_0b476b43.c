//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ARRAY_SIZE 100

double compute_polynomial(double *coeffs, int degree, double x) {
    double result = 0.0;
    double *ptr = coeffs + degree;
    
    while (ptr >= coeffs) {
        result = result * x + *ptr;
        ptr--;
    }
    
    return result;
}

void fill_with_primes(int *arr, int size) {
    if (size < 1) return;
    
    int count = 0;
    int num = 2;
    int *ptr = arr;
    
    while (count < size) {
        int is_prime = 1;
        int *check_ptr = arr;
        
        for (int i = 0; i < count; i++) {
            if (num % *check_ptr == 0) {
                is_prime = 0;
                break;
            }
            check_ptr++;
        }
        
        if (is_prime) {
            *ptr = num;
            ptr++;
            count++;
        }
        num++;
    }
}

double calculate_statistics(int *data, int size, double *mean, double *stddev) {
    if (size <= 0 || !data || !mean || !stddev) {
        return 0.0;
    }
    
    double sum = 0.0;
    double sum_sq = 0.0;
    int *ptr = data;
    
    for (int i = 0; i < size; i++) {
        sum += *ptr;
        sum_sq += (*ptr) * (*ptr);
        ptr++;
    }
    
    *mean = sum / size;
    *stddev = sqrt((sum_sq / size) - (*mean * *mean));
    
    return *stddev;
}

int main() {
    int numbers[ARRAY_SIZE];
    double coefficients[] = {2.0, -3.0, 1.0, 5.0};
    int coeff_count = sizeof(coefficients) / sizeof(coefficients[0]);
    
    fill_with_primes(numbers, ARRAY_SIZE);
    
    double mean, stddev;
    calculate_statistics(numbers, ARRAY_SIZE, &mean, &stddev);
    
    printf("First 10 prime numbers: ");
    int *num_ptr = numbers;
    for (int i = 0; i < 10 && i < ARRAY_SIZE; i++) {
        printf("%d ", *num_ptr);
        num_ptr++;
    }
    printf("\n");
    
    printf("Statistics: mean = %.2f, stddev = %.2f\n", mean, stddev);
    
    printf("Polynomial evaluation:\n");
    double *coeff_ptr = coefficients;
    printf("Coefficients: ");
    for (int i = 0; i < coeff_count; i++) {
        printf("%.1f ", *coeff_ptr);
        coeff_ptr++;
    }
    printf("\n");
    
    for (double x = -2.0; x <= 2.0; x += 0.5) {
        double result = compute_polynomial(coefficients, coeff_count - 1, x);
        printf("f(%.1f) = %.2f\n", x, result);
    }
    
    return 0;
}