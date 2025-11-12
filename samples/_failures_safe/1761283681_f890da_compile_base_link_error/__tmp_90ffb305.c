//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ARRAY_SIZE 100

double compute_mean(double *data, size_t n) {
    if (data == NULL || n == 0) return 0.0;
    double sum = 0.0;
    double *end = data + n;
    for (double *ptr = data; ptr < end; ptr++) {
        sum += *ptr;
    }
    return sum / (double)n;
}

double compute_variance(double *data, size_t n, double mean) {
    if (data == NULL || n <= 1) return 0.0;
    double sum_sq = 0.0;
    double *end = data + n;
    for (double *ptr = data; ptr < end; ptr++) {
        double diff = *ptr - mean;
        sum_sq += diff * diff;
    }
    return sum_sq / (double)(n - 1);
}

void normalize_data(double *data, size_t n, double mean, double stddev) {
    if (data == NULL || n == 0 || stddev == 0.0) return;
    double *end = data + n;
    for (double *ptr = data; ptr < end; ptr++) {
        *ptr = (*ptr - mean) / stddev;
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
        if (!isfinite(value)) {
            printf("Invalid input: non-finite number\n");
            return 1;
        }
        data[count] = value;
        count++;
    }
    
    if (count == 0) {
        printf("No valid numbers entered\n");
        return 1;
    }
    
    double mean = compute_mean(data, count);
    double variance = compute_variance(data, count, mean);
    double stddev = sqrt(variance);
    
    printf("Statistics:\n");
    printf("Count: %zu\n", count);
    printf("Mean: %.6f\n", mean);
    printf("Variance: %.6f\n", variance);
    printf("Standard deviation: %.6f\n", stddev);
    
    if (stddev > 0.0) {
        normalize_data(data, count, mean, stddev);
        
        printf("\nNormalized data:\n");
        double *end = data + count;
        for (double *ptr = data; ptr < end; ptr++) {
            printf("%.6f ", *ptr);
        }
        printf("\n");
    } else {
        printf("\nCannot normalize data (standard deviation is zero)\n");
    }
    
    return 0;
}