//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 100

int main(void) {
    double data[MAX_SIZE];
    double *ptr = data;
    double *end_ptr = data + MAX_SIZE;
    int count = 0;
    
    printf("Enter number of values to process (1-%d): ", MAX_SIZE);
    if (scanf("%d", &count) != 1) {
        fprintf(stderr, "Error reading input count\n");
        return EXIT_FAILURE;
    }
    
    if (count <= 0 || count > MAX_SIZE) {
        fprintf(stderr, "Invalid count: must be between 1 and %d\n", MAX_SIZE);
        return EXIT_FAILURE;
    }
    
    printf("Enter %d floating-point values:\n", count);
    for (double *current = ptr; current < ptr + count; current++) {
        if (scanf("%lf", current) != 1) {
            fprintf(stderr, "Error reading value %td\n", current - ptr + 1);
            return EXIT_FAILURE;
        }
    }
    
    double sum = 0.0;
    double *sum_ptr = ptr;
    while (sum_ptr < ptr + count) {
        sum += *sum_ptr;
        sum_ptr++;
    }
    
    double mean = sum / count;
    
    double variance_sum = 0.0;
    double *var_ptr = ptr;
    while (var_ptr < ptr + count) {
        double diff = *var_ptr - mean;
        variance_sum += diff * diff;
        var_ptr++;
    }
    double variance = variance_sum / count;
    double std_dev = sqrt(variance);
    
    double min_val = *ptr;
    double max_val = *ptr;
    double *minmax_ptr = ptr + 1;
    while (minmax_ptr < ptr + count) {
        if (*minmax_ptr < min_val) {
            min_val = *minmax_ptr;
        }
        if (*minmax_ptr > max_val) {
            max_val = *minmax_ptr;
        }
        minmax_ptr++;
    }
    
    double normalized[MAX_SIZE];
    double *norm_ptr = normalized;
    double *data_ptr = ptr;
    while (data_ptr < ptr + count) {
        if (max_val != min_val) {
            *norm_ptr = (*data_ptr - min_val) / (max_val - min_val);
        } else {
            *norm_ptr = 0.0;
        }
        norm_ptr++;
        data_ptr++;
    }
    
    printf("\nStatistical Analysis Results:\n");
    printf("Count: %d\n", count);
    printf("Sum: %.6f\n", sum);
    printf("Mean: %.6f\n", mean);
    printf("Variance: %.6f\n", variance);
    printf("Standard Deviation: %.6f\n", std_dev);
    printf("Minimum: %.6f\n", min_val);
    printf("Maximum: %.6f\n", max_val);
    
    printf("\nNormalized values (0-1 scale):\n");
    double *print_ptr = normalized;
    for (int i = 0; i < count; i++) {
        printf("%.6f ", *print_ptr);
        print_ptr++;
        if ((i + 1) % 5 == 0) {
            printf("\n");
        }
    }
    if (count % 5 != 0) {
        printf("\n");
    }
    
    return EXIT_SUCCESS;
}