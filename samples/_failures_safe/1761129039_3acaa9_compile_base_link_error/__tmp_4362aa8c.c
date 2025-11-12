//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ARRAY_SIZE 100

double compute_mean(double *arr, size_t n) {
    if (arr == NULL || n == 0) return 0.0;
    double sum = 0.0;
    double *end = arr + n;
    for (double *ptr = arr; ptr < end; ptr++) {
        sum += *ptr;
    }
    return sum / n;
}

double compute_variance(double *arr, size_t n, double mean) {
    if (arr == NULL || n <= 1) return 0.0;
    double sum_sq = 0.0;
    double *end = arr + n;
    for (double *ptr = arr; ptr < end; ptr++) {
        double diff = *ptr - mean;
        sum_sq += diff * diff;
    }
    return sum_sq / (n - 1);
}

void fill_array(double *arr, size_t n) {
    if (arr == NULL || n == 0) return;
    double *end = arr + n;
    for (double *ptr = arr; ptr < end; ptr++) {
        *ptr = (double)(ptr - arr) * 1.5 + 2.0;
    }
}

int main() {
    double data[ARRAY_SIZE];
    fill_array(data, ARRAY_SIZE);
    
    double mean = compute_mean(data, ARRAY_SIZE);
    double variance = compute_variance(data, ARRAY_SIZE, mean);
    double std_dev = sqrt(variance);
    
    printf("Array statistics:\n");
    printf("Size: %d\n", ARRAY_SIZE);
    printf("Mean: %.6f\n", mean);
    printf("Variance: %.6f\n", variance);
    printf("Standard deviation: %.6f\n", std_dev);
    
    printf("\nFirst 10 elements:\n");
    double *end = data + (ARRAY_SIZE < 10 ? ARRAY_SIZE : 10);
    for (double *ptr = data; ptr < end; ptr++) {
        printf("%.2f ", *ptr);
    }
    printf("\n");
    
    return 0;
}