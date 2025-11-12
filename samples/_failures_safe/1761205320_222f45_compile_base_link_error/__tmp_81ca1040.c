//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ARRAY_SIZE 100

double compute_mean(double *arr, size_t n) {
    if (arr == NULL || n == 0) return 0.0;
    double sum = 0.0;
    for (double *p = arr; p < arr + n; p++) {
        sum += *p;
    }
    return sum / n;
}

double compute_variance(double *arr, size_t n, double mean) {
    if (arr == NULL || n <= 1) return 0.0;
    double sum_sq = 0.0;
    for (double *p = arr; p < arr + n; p++) {
        double diff = *p - mean;
        sum_sq += diff * diff;
    }
    return sum_sq / (n - 1);
}

void fill_array(double *arr, size_t n) {
    if (arr == NULL || n == 0) return;
    for (double *p = arr; p < arr + n; p++) {
        *p = (double)(p - arr) * 1.5 + 2.0;
    }
}

int main(void) {
    double data[ARRAY_SIZE];
    fill_array(data, ARRAY_SIZE);
    
    double mean = compute_mean(data, ARRAY_SIZE);
    double variance = compute_variance(data, ARRAY_SIZE, mean);
    double std_dev = sqrt(variance);
    
    printf("Array Statistics:\n");
    printf("Size: %d\n", ARRAY_SIZE);
    printf("Mean: %.6f\n", mean);
    printf("Variance: %.6f\n", variance);
    printf("Standard Deviation: %.6f\n", std_dev);
    
    printf("\nFirst 10 elements:\n");
    for (double *p = data; p < data + 10 && p < data + ARRAY_SIZE; p++) {
        printf("%.2f ", *p);
    }
    printf("\n");
    
    printf("\nLast 10 elements:\n");
    if (ARRAY_SIZE >= 10) {
        for (double *p = data + ARRAY_SIZE - 10; p < data + ARRAY_SIZE; p++) {
            printf("%.2f ", *p);
        }
    } else {
        for (double *p = data; p < data + ARRAY_SIZE; p++) {
            printf("%.2f ", *p);
        }
    }
    printf("\n");
    
    return 0;
}