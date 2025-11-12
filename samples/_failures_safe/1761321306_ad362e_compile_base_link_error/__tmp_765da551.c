//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ARRAY_SIZE 100

double compute_mean(double *arr, size_t n) {
    if (n == 0) return 0.0;
    double sum = 0.0;
    double *end = arr + n;
    for (double *p = arr; p < end; p++) {
        sum += *p;
    }
    return sum / n;
}

double compute_variance(double *arr, size_t n, double mean) {
    if (n <= 1) return 0.0;
    double sum_sq = 0.0;
    double *end = arr + n;
    for (double *p = arr; p < end; p++) {
        double diff = *p - mean;
        sum_sq += diff * diff;
    }
    return sum_sq / (n - 1);
}

void fill_array(double *arr, size_t n) {
    double *end = arr + n;
    for (double *p = arr; p < end; p++) {
        *p = (double)(p - arr) * 1.5 + 2.0;
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
    double *end = data + 10;
    for (double *p = data; p < end; p++) {
        printf("%.2f ", *p);
    }
    printf("\n");
    
    printf("\nLast 10 elements:\n");
    double *start = data + ARRAY_SIZE - 10;
    end = data + ARRAY_SIZE;
    for (double *p = start; p < end; p++) {
        printf("%.2f ", *p);
    }
    printf("\n");
    
    return 0;
}