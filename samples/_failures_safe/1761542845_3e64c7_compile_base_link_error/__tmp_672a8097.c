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
    for (double *p = arr; p < end; p++) {
        sum += *p;
    }
    return sum / n;
}

double compute_variance(double *arr, size_t n, double mean) {
    if (arr == NULL || n <= 1) return 0.0;
    double sum_sq = 0.0;
    double *end = arr + n;
    for (double *p = arr; p < end; p++) {
        double diff = *p - mean;
        sum_sq += diff * diff;
    }
    return sum_sq / (n - 1);
}

void normalize_array(double *arr, size_t n, double mean, double stddev) {
    if (arr == NULL || n == 0 || stddev == 0.0) return;
    double *end = arr + n;
    for (double *p = arr; p < end; p++) {
        *p = (*p - mean) / stddev;
    }
}

int read_doubles(double *arr, size_t max_size) {
    if (arr == NULL || max_size == 0) return 0;
    size_t count = 0;
    double value;
    printf("Enter up to %zu numbers (non-number to stop): ", max_size);
    while (count < max_size && scanf("%lf", &value) == 1) {
        *(arr + count) = value;
        count++;
    }
    while (getchar() != '\n');
    return count;
}

int main(void) {
    double data[ARRAY_SIZE];
    size_t count = read_doubles(data, ARRAY_SIZE);
    
    if (count == 0) {
        printf("No valid numbers entered.\n");
        return 1;
    }
    
    double mean = compute_mean(data, count);
    double variance = compute_variance(data, count, mean);
    double stddev = sqrt(variance);
    
    printf("Count: %zu\n", count);
    printf("Mean: %.6f\n", mean);
    printf("Variance: %.6f\n", variance);
    printf("Standard Deviation: %.6f\n", stddev);
    
    if (stddev > 0.0) {
        normalize_array(data, count, mean, stddev);
        printf("Normalized values:\n");
        double *end = data + count;
        for (double *p = data; p < end; p++) {
            printf("%.6f ", *p);
        }
        printf("\n");
    } else {
        printf("Cannot normalize (standard deviation is zero).\n");
    }
    
    return 0;
}