//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define ARRAY_SIZE 100

int main() {
    double results[ARRAY_SIZE];
    double sum = 0.0;
    double product = 1.0;
    int valid_count = 0;
    
    printf("Enter up to %d numbers (non-numeric input to stop):\n", ARRAY_SIZE);
    
    for (int i = 0; i < ARRAY_SIZE; i++) {
        char input[256];
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        char *endptr;
        double value = strtod(input, &endptr);
        
        if (endptr == input || *endptr != '\n') {
            break;
        }
        
        if (value == HUGE_VAL || value == -HUGE_VAL || value != value) {
            break;
        }
        
        results[valid_count] = value;
        valid_count++;
    }
    
    if (valid_count == 0) {
        printf("No valid numbers entered.\n");
        return 1;
    }
    
    printf("Processing %d numbers...\n", valid_count);
    
    int i = 0;
    while (i < valid_count) {
        sum += results[i];
        i++;
    }
    
    double mean = sum / valid_count;
    printf("Mean: %.6f\n", mean);
    
    int j = 0;
    double variance_sum = 0.0;
    do {
        double diff = results[j] - mean;
        variance_sum += diff * diff;
        j++;
    } while (j < valid_count);
    
    double variance = variance_sum / valid_count;
    printf("Variance: %.6f\n", variance);
    
    double min_val = results[0];
    double max_val = results[0];
    for (int k = 1; k < valid_count; k++) {
        if (results[k] < min_val) {
            min_val = results[k];
        }
        if (results[k] > max_val) {
            max_val = results[k];
        }
    }
    printf("Range: [%.6f, %.6f]\n", min_val, max_val);
    
    int count = 0;
    for (int m = 0; m < valid_count; m++) {
        if (results[m] > mean) {
            count++;
        }
    }
    printf("Numbers above mean: %d\n", count);
    
    double running_sum = 0.0;
    printf("Running sums:\n");
    for (int n = 0; n < valid_count; n++) {
        running_sum += results[n];
        printf("  After %d: %.6f\n", n + 1, running_sum);
    }
    
    return 0;
}