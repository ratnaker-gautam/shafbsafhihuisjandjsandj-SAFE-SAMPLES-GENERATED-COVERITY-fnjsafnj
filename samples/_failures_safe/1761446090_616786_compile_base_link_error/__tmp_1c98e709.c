//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ARRAY_SIZE 100

double compute_polynomial(double *coeffs, int degree, double x) {
    if (degree < 0 || coeffs == NULL) return 0.0;
    
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
    
    if (count >= 1) *arr = 0;
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
        sum += *ptr;
        ptr++;
    }
    
    return (double)sum / size;
}

int main() {
    double coefficients[5] = {2.0, -3.0, 1.5, 0.5, -1.0};
    int fibonacci_numbers[ARRAY_SIZE];
    int user_array[ARRAY_SIZE];
    
    printf("Polynomial evaluation:\n");
    for (double x = -2.0; x <= 2.0; x += 0.5) {
        double result = compute_polynomial(coefficients, 4, x);
        printf("f(%.1f) = %.3f\n", x, result);
    }
    
    printf("\nFibonacci sequence (first 20):\n");
    fill_fibonacci(fibonacci_numbers, 20);
    int *fib_ptr = fibonacci_numbers;
    for (int i = 0; i < 20; i++) {
        printf("%d ", *fib_ptr);
        fib_ptr++;
        if ((i + 1) % 10 == 0) printf("\n");
    }
    
    printf("\nEnter up to %d integers (0 to stop):\n", ARRAY_SIZE);
    int count = 0;
    int *user_ptr = user_array;
    
    while (count < ARRAY_SIZE) {
        int input;
        if (scanf("%d", &input) != 1) {
            printf("Invalid input. Using entered values.\n");
            while (getchar() != '\n');
            break;
        }
        
        if (input == 0) break;
        
        *user_ptr = input;
        user_ptr++;
        count++;
    }
    
    if (count > 0) {
        double mean = calculate_mean(user_array, count);
        printf("Mean of entered values: %.2f\n", mean);
        
        double variance_sum = 0.0;
        int *calc_ptr = user_array;
        for (int i = 0; i < count; i++) {
            double diff = *calc_ptr - mean;
            variance_sum += diff * diff;
            calc_ptr++;
        }
        double variance = variance_sum / count;
        printf("Variance: %.2f\n", variance);
        printf("Standard deviation: %.2f\n", sqrt(variance));
    } else {
        printf("No valid data entered.\n");
    }
    
    return 0;
}