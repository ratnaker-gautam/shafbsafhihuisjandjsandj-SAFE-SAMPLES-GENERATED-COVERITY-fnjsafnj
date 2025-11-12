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

void sort_array(double *arr, size_t n) {
    if (n <= 1) return;
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
    if (n == 0) return 0.0;
    double temp[ARRAY_SIZE];
    double *end = temp + n;
    double *src = arr;
    for (double *dst = temp; dst < end; dst++, src++) {
        *dst = *src;
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
        if (result != 1) {
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
    
    if (getchar() != '\n') {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }
    
    double mean = compute_mean(data, count);
    double variance = compute_variance(data, count, mean);
    double std_dev = sqrt(variance);
    double median = compute_median(data, count);
    
    printf("Statistical analysis of %zu numbers:\n", count);
    printf("Mean: %.6f\n", mean);
    printf("Variance: %.6f\n", variance);
    printf("Standard deviation: %.6f\n", std_dev);
    printf("Median: %.6f\n", median);
    
    double *min_ptr = data;
    double *max_ptr = data;
    double *end = data + count;
    
    for (double *p = data + 1; p < end; p++) {
        if (*p < *min_ptr) min_ptr = p;
        if (*p > *max_ptr) max_ptr = p;
    }
    
    printf("Minimum: %.6f (at position %ld)\n", *min_ptr, min_ptr - data);
    printf("Maximum: %.6f (at position %ld)\n", *max_ptr, max_ptr - data);
    
    return 0;
}