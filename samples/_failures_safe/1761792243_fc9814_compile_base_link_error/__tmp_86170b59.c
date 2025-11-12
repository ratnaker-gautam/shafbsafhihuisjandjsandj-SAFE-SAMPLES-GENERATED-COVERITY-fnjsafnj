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
    
    printf("Enter up to %d numbers (non-number to stop): ", MAX_SIZE);
    
    while (ptr < end_ptr) {
        double value;
        if (scanf("%lf", &value) != 1) {
            break;
        }
        
        if (!isfinite(value)) {
            printf("Invalid input: non-finite number\n");
            return 1;
        }
        
        *ptr = value;
        ptr++;
        count++;
        
        if (count >= MAX_SIZE) {
            break;
        }
    }
    
    if (count == 0) {
        printf("No valid numbers entered\n");
        return 1;
    }
    
    double *current = data;
    double sum = 0.0;
    double min_val = *current;
    double max_val = *current;
    
    while (current < data + count) {
        sum += *current;
        if (*current < min_val) min_val = *current;
        if (*current > max_val) max_val = *current;
        current++;
    }
    
    double mean = sum / count;
    
    current = data;
    double variance_sum = 0.0;
    while (current < data + count) {
        double diff = *current - mean;
        variance_sum += diff * diff;
        current++;
    }
    double variance = variance_sum / count;
    double std_dev = sqrt(variance);
    
    printf("Statistics for %d numbers:\n", count);
    printf("Sum: %.6f\n", sum);
    printf("Mean: %.6f\n", mean);
    printf("Min: %.6f\n", min_val);
    printf("Max: %.6f\n", max_val);
    printf("Standard deviation: %.6f\n", std_dev);
    
    double threshold;
    printf("Enter threshold for filtering: ");
    if (scanf("%lf", &threshold) != 1) {
        printf("Invalid threshold\n");
        return 1;
    }
    
    if (!isfinite(threshold)) {
        printf("Invalid threshold: non-finite number\n");
        return 1;
    }
    
    printf("Numbers above threshold %.6f:\n", threshold);
    int above_count = 0;
    current = data;
    while (current < data + count) {
        if (*current > threshold) {
            printf("%.6f ", *current);
            above_count++;
        }
        current++;
    }
    
    if (above_count == 0) {
        printf("None");
    }
    printf("\n");
    
    return 0;
}