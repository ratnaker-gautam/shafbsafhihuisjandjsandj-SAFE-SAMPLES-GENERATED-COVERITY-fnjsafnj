//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ARRAY_SIZE 100

double compute_mean(double *data, size_t n) {
    if (data == NULL || n == 0) return 0.0;
    double sum = 0.0;
    double *ptr = data;
    double *end = data + n;
    while (ptr < end) {
        sum += *ptr;
        ptr++;
    }
    return sum / (double)n;
}

double compute_variance(double *data, size_t n, double mean) {
    if (data == NULL || n <= 1) return 0.0;
    double sum_sq = 0.0;
    double *ptr = data;
    double *end = data + n;
    while (ptr < end) {
        double diff = *ptr - mean;
        sum_sq += diff * diff;
        ptr++;
    }
    return sum_sq / (double)(n - 1);
}

void sort_array(double *data, size_t n) {
    if (data == NULL || n <= 1) return;
    double *ptr1 = data;
    double *end = data + n;
    while (ptr1 < end - 1) {
        double *ptr2 = ptr1 + 1;
        while (ptr2 < end) {
            if (*ptr1 > *ptr2) {
                double temp = *ptr1;
                *ptr1 = *ptr2;
                *ptr2 = temp;
            }
            ptr2++;
        }
        ptr1++;
    }
}

double compute_median(double *data, size_t n) {
    if (data == NULL || n == 0) return 0.0;
    double temp[ARRAY_SIZE];
    if (n > ARRAY_SIZE) return 0.0;
    double *ptr_temp = temp;
    double *ptr_data = data;
    double *end = data + n;
    while (ptr_data < end) {
        *ptr_temp = *ptr_data;
        ptr_temp++;
        ptr_data++;
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
    
    printf("Enter up to %d numbers (non-number to stop):\n", ARRAY_SIZE);
    
    while (count < ARRAY_SIZE) {
        double value;
        int result = scanf("%lf", &value);
        if (result != 1) break;
        data[count] = value;
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
    
    double *ptr = data;
    double *end = data + count;
    printf("Sorted data: ");
    while (ptr < end) {
        printf("%.2f ", *ptr);
        ptr++;
    }
    printf("\n");
    
    return 0;
}