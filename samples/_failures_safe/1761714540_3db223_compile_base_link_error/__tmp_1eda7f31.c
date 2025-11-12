//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ARRAY_SIZE 100

double compute_mean(double *data, size_t n) {
    if (data == NULL || n == 0) return 0.0;
    double sum = 0.0;
    for (size_t i = 0; i < n; i++) {
        sum += *(data + i);
    }
    return sum / n;
}

double compute_variance(double *data, size_t n, double mean) {
    if (data == NULL || n <= 1) return 0.0;
    double sum_sq = 0.0;
    for (size_t i = 0; i < n; i++) {
        double diff = *(data + i) - mean;
        sum_sq += diff * diff;
    }
    return sum_sq / (n - 1);
}

void sort_array(double *arr, size_t n) {
    if (arr == NULL || n == 0) return;
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

double compute_median(double *data, size_t n) {
    if (data == NULL || n == 0) return 0.0;
    double *copy = malloc(n * sizeof(double));
    if (copy == NULL) return 0.0;
    for (size_t i = 0; i < n; i++) {
        *(copy + i) = *(data + i);
    }
    sort_array(copy, n);
    double median;
    if (n % 2 == 0) {
        median = (*(copy + n/2 - 1) + *(copy + n/2)) / 2.0;
    } else {
        median = *(copy + n/2);
    }
    free(copy);
    return median;
}

int main(void) {
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
    
    double mean = compute_mean(data, count);
    double variance = compute_variance(data, count, mean);
    double std_dev = sqrt(variance);
    double median = compute_median(data, count);
    
    printf("Statistical Analysis:\n");
    printf("Count: %zu\n", count);
    printf("Mean: %.6f\n", mean);
    printf("Variance: %.6f\n", variance);
    printf("Standard Deviation: %.6f\n", std_dev);
    printf("Median: %.6f\n", median);
    
    double *min_ptr = data;
    double *max_ptr = data;
    for (size_t i = 1; i < count; i++) {
        if (*(data + i) < *min_ptr) min_ptr = data + i;
        if (*(data + i) > *max_ptr) max_ptr = data + i;
    }
    
    printf("Minimum: %.6f\n", *min_ptr);
    printf("Maximum: %.6f\n", *max_ptr);
    
    return 0;
}