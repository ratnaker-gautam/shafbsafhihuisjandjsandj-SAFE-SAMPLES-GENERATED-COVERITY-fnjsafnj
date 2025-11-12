//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ARRAY_SIZE 100

double compute_mean(double *data, size_t n) {
    if (data == NULL || n == 0) return 0.0;
    double sum = 0.0;
    double *end = data + n;
    for (double *ptr = data; ptr < end; ptr++) {
        sum += *ptr;
    }
    return sum / n;
}

double compute_variance(double *data, size_t n, double mean) {
    if (data == NULL || n <= 1) return 0.0;
    double sum_sq = 0.0;
    double *end = data + n;
    for (double *ptr = data; ptr < end; ptr++) {
        double diff = *ptr - mean;
        sum_sq += diff * diff;
    }
    return sum_sq / (n - 1);
}

void fill_array(double *arr, size_t n) {
    if (arr == NULL || n == 0) return;
    double *end = arr + n;
    for (double *ptr = arr; ptr < end; ptr++) {
        *ptr = (double)(ptr - arr) * 1.5 + 2.0;
    }
}

int main() {
    double data[ARRAY_SIZE];
    fill_array(data, ARRAY_SIZE);
    
    double mean = compute_mean(data, ARRAY_SIZE);
    double variance = compute_variance(data, ARRAY_SIZE, mean);
    double std_dev = sqrt(variance);
    
    printf("Array size: %d\n", ARRAY_SIZE);
    printf("Mean: %.6f\n", mean);
    printf("Variance: %.6f\n", variance);
    printf("Standard deviation: %.6f\n", std_dev);
    
    double test_value;
    printf("Enter a value to find in array: ");
    if (scanf("%lf", &test_value) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    int found = 0;
    double *end = data + ARRAY_SIZE;
    for (double *ptr = data; ptr < end; ptr++) {
        if (fabs(*ptr - test_value) < 1e-9) {
            printf("Value %.6f found at position %ld\n", test_value, ptr - data);
            found = 1;
            break;
        }
    }
    
    if (!found) {
        printf("Value %.6f not found in array\n", test_value);
    }
    
    double threshold;
    printf("Enter threshold for values above: ");
    if (scanf("%lf", &threshold) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("Values above %.6f:\n", threshold);
    int count = 0;
    for (double *ptr = data; ptr < end; ptr++) {
        if (*ptr > threshold) {
            printf("%.6f ", *ptr);
            count++;
        }
    }
    printf("\nFound %d values above threshold\n", count);
    
    return 0;
}