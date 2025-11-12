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
    return sum / (double)n;
}

double compute_variance(double *data, size_t n, double mean) {
    if (data == NULL || n <= 1) return 0.0;
    double sum_sq = 0.0;
    for (size_t i = 0; i < n; i++) {
        double diff = *(data + i) - mean;
        sum_sq += diff * diff;
    }
    return sum_sq / (double)(n - 1);
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
    if (n % 2 == 0) {
        return (*(data + n/2 - 1) + *(data + n/2)) / 2.0;
    } else {
        return *(data + n/2);
    }
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
        if (!isfinite(value)) {
            printf("Invalid input: non-finite number\n");
            return 1;
        }
        *(data + count) = value;
        count++;
    }
    
    if (count == 0) {
        printf("No valid numbers entered\n");
        return 1;
    }
    
    if (count > ARRAY_SIZE) {
        printf("Input count exceeds array size\n");
        return 1;
    }
    
    double mean = compute_mean(data, count);
    double variance = compute_variance(data, count, mean);
    double std_dev = sqrt(variance);
    
    double temp_data[ARRAY_SIZE];
    for (size_t i = 0; i < count; i++) {
        *(temp_data + i) = *(data + i);
    }
    sort_array(temp_data, count);
    double median = compute_median(temp_data, count);
    
    printf("Statistical Analysis Results:\n");
    printf("Count: %zu\n", count);
    printf("Mean: %.6f\n", mean);
    printf("Variance: %.6f\n", variance);
    printf("Standard Deviation: %.6f\n", std_dev);
    printf("Median: %.6f\n", median);
    
    printf("Sorted data: ");
    for (size_t i = 0; i < count; i++) {
        printf("%.2f ", *(temp_data + i));
    }
    printf("\n");
    
    return 0;
}