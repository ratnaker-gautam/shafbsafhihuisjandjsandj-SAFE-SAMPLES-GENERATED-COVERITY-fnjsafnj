//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

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

void fill_fibonacci(int *arr, size_t size) {
    if (arr == NULL || size < 2) return;
    
    *arr = 0;
    *(arr + 1) = 1;
    
    for (size_t i = 2; i < size; i++) {
        int sum = *(arr + i - 1) + *(arr + i - 2);
        if ((*(arr + i - 1) > 0 && *(arr + i - 2) > 0 && sum < 0) ||
            (*(arr + i - 1) < 0 && *(arr + i - 2) < 0 && sum > 0)) {
            break;
        }
        *(arr + i) = sum;
    }
}

double calculate_mean(int *data, size_t count) {
    if (data == NULL || count == 0) return 0.0;
    
    long long sum = 0;
    for (size_t i = 0; i < count; i++) {
        sum += *(data + i);
        if (sum > LLONG_MAX - 1000 || sum < LLONG_MIN + 1000) {
            return 0.0;
        }
    }
    
    return (double)sum / count;
}

int main() {
    double coefficients[5] = {2.5, -1.3, 0.7, -0.2, 1.0};
    int degree = 4;
    
    printf("Polynomial evaluation:\n");
    for (double x = -2.0; x <= 2.0; x += 0.5) {
        double result = compute_polynomial(coefficients, degree, x);
        printf("f(%.1f) = %.3f\n", x, result);
    }
    
    printf("\nFibonacci sequence:\n");
    int fibonacci[ARRAY_SIZE];
    memset(fibonacci, 0, sizeof(fibonacci));
    
    fill_fibonacci(fibonacci, ARRAY_SIZE);
    
    for (size_t i = 0; i < ARRAY_SIZE; i++) {
        if (i > 0 && *(fibonacci + i) == 0 && *(fibonacci + i - 1) != 0) break;
        printf("%d ", *(fibonacci + i));
        if ((i + 1) % 10 == 0) printf("\n");
    }
    printf("\n");
    
    printf("\nStatistical analysis:\n");
    int sample_data[20];
    for (int i = 0; i < 20; i++) {
        *(sample_data + i) = (i * 7 + 3) % 23;
    }
    
    double mean = calculate_mean(sample_data, 20);
    printf("Mean of sample data: %.2f\n", mean);
    
    double variance_sum = 0.0;
    for (int i = 0; i < 20; i++) {
        double diff = *(sample_data + i) - mean;
        variance_sum += diff * diff;
    }
    double variance = variance_sum / 20;
    printf("Variance: %.2f\n", variance);
    printf("Standard deviation: %.2f\n", sqrt(variance));
    
    return 0;
}