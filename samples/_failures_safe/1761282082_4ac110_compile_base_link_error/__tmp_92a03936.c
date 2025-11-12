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
    return sum / n;
}

double compute_stddev(double *arr, size_t n, double mean) {
    if (arr == NULL || n <= 1) return 0.0;
    double sum_sq = 0.0;
    for (size_t i = 0; i < n; i++) {
        double diff = *(arr + i) - mean;
        sum_sq += diff * diff;
    }
    return sqrt(sum_sq / (n - 1));
}

void normalize_array(double *arr, size_t n, double mean, double stddev) {
    if (arr == NULL || n == 0 || stddev == 0.0) return;
    for (size_t i = 0; i < n; i++) {
        *(arr + i) = (*(arr + i) - mean) / stddev;
    }
}

int main(void) {
    double data[ARRAY_SIZE];
    size_t count = 0;
    
    printf("Enter up to %zu numbers (non-number to stop):\n", ARRAY_SIZE);
    
    while (count < ARRAY_SIZE) {
        double value;
        if (scanf("%lf", &value) != 1) {
            break;
        }
        *(data + count) = value;
        count++;
    }
    
    if (count == 0) {
        printf("No valid numbers entered.\n");
        return 1;
    }
    
    if (count > ARRAY_SIZE) {
        count = ARRAY_SIZE;
    }
    
    double mean = compute_mean(data, count);
    double stddev = compute_stddev(data, count, mean);
    
    printf("Original data:\n");
    for (size_t i = 0; i < count; i++) {
        printf("%.6f ", *(data + i));
    }
    printf("\n");
    
    printf("Mean: %.6f\n", mean);
    printf("Standard deviation: %.6f\n", stddev);
    
    if (stddev != 0.0) {
        normalize_array(data, count, mean, stddev);
        
        printf("Normalized data (z-scores):\n");
        for (size_t i = 0; i < count; i++) {
            printf("%.6f ", *(data + i));
        }
        printf("\n");
        
        double normalized_mean = compute_mean(data, count);
        double normalized_stddev = compute_stddev(data, count, normalized_mean);
        printf("Normalized mean: %.6f\n", normalized_mean);
        printf("Normalized standard deviation: %.6f\n", normalized_stddev);
    } else {
        printf("Cannot normalize: standard deviation is zero.\n");
    }
    
    return 0;
}