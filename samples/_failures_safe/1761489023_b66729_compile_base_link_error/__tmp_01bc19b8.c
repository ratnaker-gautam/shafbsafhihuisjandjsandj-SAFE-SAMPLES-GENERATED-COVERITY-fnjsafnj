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

double compute_variance(double *arr, size_t n, double mean) {
    if (arr == NULL || n <= 1) return 0.0;
    double sum_sq = 0.0;
    for (size_t i = 0; i < n; i++) {
        double diff = *(arr + i) - mean;
        sum_sq += diff * diff;
    }
    return sum_sq / (n - 1);
}

void sort_array(double *arr, size_t n) {
    if (arr == NULL || n <= 1) return;
    for (size_t i = 0; i < n - 1; i++) {
        for (size_t j = 0; j < n - i - 1; j++) {
            if (*(arr + j) > *(arr + j + 1)) {
                double temp = *(arr + j);
                *(arr + j) = *(arr + j + 1);
                *(arr + j + 1) = temp;
            }
        }
    }
}

int main() {
    double data[ARRAY_SIZE];
    size_t count = 0;
    
    printf("Enter up to %zu numbers (non-number to stop):\n", ARRAY_SIZE);
    
    while (count < ARRAY_SIZE) {
        double value;
        int result = scanf("%lf", &value);
        if (result != 1) break;
        *(data + count) = value;
        count++;
    }
    
    if (count == 0) {
        printf("No valid numbers entered.\n");
        return 1;
    }
    
    printf("Entered %zu numbers.\n", count);
    
    double mean = compute_mean(data, count);
    double variance = compute_variance(data, count, mean);
    double std_dev = sqrt(variance);
    
    printf("Mean: %.6f\n", mean);
    printf("Variance: %.6f\n", variance);
    printf("Standard deviation: %.6f\n", std_dev);
    
    sort_array(data, count);
    
    printf("Sorted values:\n");
    for (size_t i = 0; i < count; i++) {
        printf("%.6f ", *(data + i));
        if ((i + 1) % 5 == 0) printf("\n");
    }
    if (count % 5 != 0) printf("\n");
    
    double median;
    if (count % 2 == 0) {
        median = (*(data + count/2 - 1) + *(data + count/2)) / 2.0;
    } else {
        median = *(data + count/2);
    }
    printf("Median: %.6f\n", median);
    
    return 0;
}