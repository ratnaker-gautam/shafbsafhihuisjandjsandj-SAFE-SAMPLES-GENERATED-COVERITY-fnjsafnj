//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ARRAY_SIZE 100

double compute_mean(double *arr, size_t n) {
    if (arr == NULL || n == 0) return 0.0;
    double sum = 0.0;
    for (size_t i = 0; i < n; i++) {
        sum += *(arr + i);
    }
    return sum / (double)n;
}

double compute_variance(double *arr, size_t n, double mean) {
    if (arr == NULL || n <= 1) return 0.0;
    double sum_sq = 0.0;
    for (size_t i = 0; i < n; i++) {
        double diff = *(arr + i) - mean;
        sum_sq += diff * diff;
    }
    return sum_sq / (double)(n - 1);
}

void fill_array(double *arr, size_t n) {
    if (arr == NULL || n == 0) return;
    for (size_t i = 0; i < n; i++) {
        *(arr + i) = (double)(i * i) / 10.0;
    }
}

int main() {
    double data[ARRAY_SIZE];
    fill_array(data, ARRAY_SIZE);
    
    double mean = compute_mean(data, ARRAY_SIZE);
    double variance = compute_variance(data, ARRAY_SIZE, mean);
    double std_dev = sqrt(variance);
    
    printf("Array statistics:\n");
    printf("Mean: %.6f\n", mean);
    printf("Variance: %.6f\n", variance);
    printf("Standard deviation: %.6f\n", std_dev);
    
    printf("\nFirst 10 elements:\n");
    for (size_t i = 0; i < 10 && i < ARRAY_SIZE; i++) {
        printf("data[%zu] = %.6f\n", i, *(data + i));
    }
    
    return 0;
}