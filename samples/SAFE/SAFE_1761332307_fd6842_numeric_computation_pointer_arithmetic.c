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

double compute_median(double *arr, size_t n) {
    if (arr == NULL || n == 0) return 0.0;
    double temp[ARRAY_SIZE];
    if (n > ARRAY_SIZE) return 0.0;
    for (size_t i = 0; i < n; i++) {
        temp[i] = *(arr + i);
    }
    sort_array(temp, n);
    if (n % 2 == 0) {
        return (temp[n/2 - 1] + temp[n/2]) / 2.0;
    } else {
        return temp[n/2];
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
    
    while (getchar() != '\n');
    
    double mean = compute_mean(data, count);
    double variance = compute_variance(data, count, mean);
    double median = compute_median(data, count);
    
    printf("Statistical Analysis:\n");
    printf("Count: %zu\n", count);
    printf("Mean: %.6f\n", mean);
    printf("Variance: %.6f\n", variance);
    printf("Median: %.6f\n", median);
    
    printf("\nSorted data:\n");
    double sorted[ARRAY_SIZE];
    if (count <= ARRAY_SIZE) {
        for (size_t i = 0; i < count; i++) {
            sorted[i] = *(data + i);
        }
        sort_array(sorted, count);
        for (size_t i = 0; i < count; i++) {
            printf("%.6f ", *(sorted + i));
        }
        printf("\n");
    }
    
    return 0;
}