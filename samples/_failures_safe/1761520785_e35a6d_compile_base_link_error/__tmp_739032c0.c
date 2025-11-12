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
    
    double *min_ptr = ptr;
    double *max_ptr = ptr;
    double *search_ptr = ptr + 1;
    while (search_ptr < ptr + count) {
        if (*search_ptr < *min_ptr) {
            min_ptr = search_ptr;
        }
        if (*search_ptr > *max_ptr) {
            max_ptr = search_ptr;
        }
        search_ptr++;
    }
    
    printf("Statistical Analysis Results:\n");
    printf("Count: %d\n", count);
    printf("Sum: %.6f\n", sum);
    printf("Mean: %.6f\n", mean);
    printf("Variance: %.6f\n", variance);
    printf("Standard Deviation: %.6f\n", std_dev);
    printf("Minimum: %.6f\n", *min_ptr);
    printf("Maximum: %.6f\n", *max_ptr);
    
    double threshold;
    printf("Enter threshold for filtering: ");
    if (scanf("%lf", &threshold) != 1) {
        fprintf(stderr, "Error: Invalid threshold\n");
        return EXIT_FAILURE;
    }
    
    printf("Values above threshold %.6f:\n", threshold);
    int above_count = 0;
    double *filter_ptr = ptr;
    while (filter_ptr < ptr + count) {
        if (*filter_ptr > threshold) {
            printf("%.6f ", *filter_ptr);
            above_count++;
        }
        filter_ptr++;
    }
    printf("\nTotal above threshold: %d\n", above_count);
    
    return EXIT_SUCCESS;
}