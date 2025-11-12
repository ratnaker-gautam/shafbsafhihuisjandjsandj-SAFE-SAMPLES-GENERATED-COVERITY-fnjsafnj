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

double compute_stddev(double *arr, size_t n) {
    if (arr == NULL || n < 2) return 0.0;
    double mean = compute_mean(arr, n);
    double sum_sq = 0.0;
    double *end = arr + n;
    for (double *p = arr; p < end; p++) {
        double diff = *p - mean;
        sum_sq += diff * diff;
    }
    return sqrt(sum_sq / (n - 1));
}

void sort_array(double *arr, size_t n) {
    if (arr == NULL || n < 2) return;
    double *end = arr + n;
    for (double *i = arr; i < end - 1; i++) {
        for (double *j = i + 1; j < end; j++) {
            if (*i > *j) {
                double temp = *i;
                *i = *j;
                *j = temp;
            }
        }
    }
}

double compute_median(double *arr, size_t n) {
    if (arr == NULL || n == 0) return 0.0;
    double *temp = malloc(n * sizeof(double));
    if (temp == NULL) return 0.0;
    
    double *src = arr;
    double *dst = temp;
    double *end = arr + n;
    while (src < end) {
        *dst = *src;
        src++;
        dst++;
    }
    
    sort_array(temp, n);
    
    double result;
    if (n % 2 == 0) {
        double *mid1 = temp + (n / 2 - 1);
        double *mid2 = temp + (n / 2);
        result = (*mid1 + *mid2) / 2.0;
    } else {
        double *mid = temp + (n / 2);
        result = *mid;
    }
    
    free(temp);
    return result;
}

int main() {
    double data[ARRAY_SIZE];
    size_t count = 0;
    
    printf("Enter up to %zu numbers (non-number to stop):\n", ARRAY_SIZE);
    
    while (count < ARRAY_SIZE) {
        double value;
        if (scanf("%lf", &value) != 1) {
            break;
        }
        data[count] = value;
        count++;
    }
    
    if (count == 0) {
        printf("No valid numbers entered.\n");
        return 1;
    }
    
    printf("\nStatistical Analysis:\n");
    printf("Count: %zu\n", count);
    
    double mean = compute_mean(data, count);
    printf("Mean: %.6f\n", mean);
    
    double stddev = compute_stddev(data, count);
    printf("Standard Deviation: %.6f\n", stddev);
    
    double median = compute_median(data, count);
    printf("Median: %.6f\n", median);
    
    double min = data[0];
    double max = data[0];
    double *end = data + count;
    for (double *p = data; p < end; p++) {
        if (*p < min) min = *p;
        if (*p > max) max = *p;
    }
    printf("Range: [%.6f, %.6f]\n", min, max);
    
    return 0;
}