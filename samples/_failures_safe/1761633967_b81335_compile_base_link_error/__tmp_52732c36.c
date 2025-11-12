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

void fill_array(double *arr, size_t n) {
    if (arr == NULL || n == 0) return;
    double *end = arr + n;
    for (double *p = arr; p < end; p++) {
        *p = (double)(p - arr) * 1.5 + 2.0;
    }
}

int main(void) {
    double data[ARRAY_SIZE];
    fill_array(data, ARRAY_SIZE);
    
    double mean = compute_mean(data, ARRAY_SIZE);
    double variance = compute_variance(data, ARRAY_SIZE, mean);
    double std_dev = sqrt(variance);
    
    printf("Array size: %d\n", ARRAY_SIZE);
    printf("Mean: %.6f\n", mean);
    printf("Variance: %.6f\n", variance);
    printf("Standard deviation: %.6f\n", std_dev);
    
    double threshold = mean + 2.0 * std_dev;
    printf("Outlier threshold: %.6f\n", threshold);
    
    int outlier_count = 0;
    double *end = data + ARRAY_SIZE;
    for (double *p = data; p < end; p++) {
        if (*p > threshold) {
            outlier_count++;
        }
    }
    
    printf("Outliers found: %d\n", outlier_count);
    
    double search_value = mean;
    int found_count = 0;
    for (double *p = data; p < end; p++) {
        if (fabs(*p - search_value) < 1e-6) {
            found_count++;
        }
    }
    
    printf("Values equal to mean: %d\n", found_count);
    
    return 0;
}