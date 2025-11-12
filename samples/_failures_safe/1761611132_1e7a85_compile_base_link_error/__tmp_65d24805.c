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
        fprintf(stderr, "Error: Invalid input\n");
        return EXIT_FAILURE;
    }
    
    if (count <= 0 || count > MAX_SIZE) {
        fprintf(stderr, "Error: Count out of range\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter %d floating-point values:\n", count);
    for (double *p = ptr; p < ptr + count; p++) {
        if (scanf("%lf", p) != 1) {
            fprintf(stderr, "Error: Invalid input\n");
            return EXIT_FAILURE;
        }
    }
    
    double sum = 0.0;
    for (double *p = ptr; p < ptr + count; p++) {
        sum += *p;
    }
    
    double mean = sum / count;
    
    double variance_sum = 0.0;
    for (double *p = ptr; p < ptr + count; p++) {
        double diff = *p - mean;
        variance_sum += diff * diff;
    }
    
    double variance = variance_sum / count;
    double std_dev = sqrt(variance);
    
    double *min_ptr = ptr;
    double *max_ptr = ptr;
    
    for (double *p = ptr + 1; p < ptr + count; p++) {
        if (*p < *min_ptr) {
            min_ptr = p;
        }
        if (*p > *max_ptr) {
            max_ptr = p;
        }
    }
    
    printf("\nStatistical Analysis Results:\n");
    printf("Count: %d\n", count);
    printf("Sum: %.6f\n", sum);
    printf("Mean: %.6f\n", mean);
    printf("Variance: %.6f\n", variance);
    printf("Standard Deviation: %.6f\n", std_dev);
    printf("Minimum: %.6f\n", *min_ptr);
    printf("Maximum: %.6f\n", *max_ptr);
    
    printf("\nNormalized values (mean=0, std_dev=1):\n");
    for (double *p = ptr; p < ptr + count; p++) {
        double normalized = (std_dev != 0.0) ? (*p - mean) / std_dev : 0.0;
        printf("%.6f ", normalized);
    }
    printf("\n");
    
    return EXIT_SUCCESS;
}