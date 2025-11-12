//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
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

void sort_array(double *data, size_t n) {
    if (data == NULL || n <= 1) return;
    for (size_t i = 0; i < n - 1; i++) {
        for (size_t j = 0; j < n - i - 1; j++) {
            if (*(data + j) > *(data + j + 1)) {
                double temp = *(data + j);
                *(data + j) = *(data + j + 1);
                *(data + j + 1) = temp;
            }
        }
    }
}

double compute_median(double *data, size_t n) {
    if (data == NULL || n == 0) return 0.0;
    double temp[ARRAY_SIZE];
    if (n > ARRAY_SIZE) return 0.0;
    for (size_t i = 0; i < n; i++) {
        temp[i] = *(data + i);
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
    
    double mean = compute_mean(data, count);
    double variance = compute_variance(data, count, mean);
    double median = compute_median(data, count);
    
    printf("Statistical Analysis:\n");
    printf("Count: %zu\n", count);
    printf("Mean: %.6f\n", mean);
    printf("Variance: %.6f\n", variance);
    printf("Median: %.6f\n", median);
    
    printf("\nSorted data:\n");
    double temp[ARRAY_SIZE];
    if (count <= ARRAY_SIZE) {
        for (size_t i = 0; i < count; i++) {
            temp[i] = *(data + i);
        }
        sort_array(temp, count);
        for (size_t i = 0; i < count; i++) {
            printf("%.6f ", temp[i]);
        }
        printf("\n");
    }
    
    return 0;
}