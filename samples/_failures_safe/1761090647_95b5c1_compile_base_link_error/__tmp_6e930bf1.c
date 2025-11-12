//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000
#define ARRAY_SIZE 100

int main(void) {
    double results[ARRAY_SIZE];
    int valid_count = 0;
    double input_value;
    char input_buffer[256];
    
    printf("Enter a positive number for computation: ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (sscanf(input_buffer, "%lf", &input_value) != 1) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    if (input_value <= 0.0 || input_value > 1e6) {
        fprintf(stderr, "Input must be positive and less than 1,000,000\n");
        return 1;
    }
    
    int i = 0;
    while (i < ARRAY_SIZE) {
        double current = input_value;
        int iterations = 0;
        
        do {
            if (iterations >= MAX_ITERATIONS) {
                break;
            }
            current = sqrt(current) + log(current + 1.0);
            if (current <= 0.0 || !isfinite(current)) {
                break;
            }
            iterations++;
        } while (current > 1.0 && iterations < MAX_ITERATIONS);
        
        if (current > 0.0 && isfinite(current)) {
            results[valid_count] = current;
            valid_count++;
        }
        i++;
    }
    
    if (valid_count == 0) {
        printf("No valid results computed\n");
        return 1;
    }
    
    double sum = 0.0;
    double min_val = results[0];
    double max_val = results[0];
    
    for (int j = 0; j < valid_count; j++) {
        sum += results[j];
        if (results[j] < min_val) min_val = results[j];
        if (results[j] > max_val) max_val = results[j];
    }
    
    double mean = sum / valid_count;
    
    double variance_sum = 0.0;
    int k = 0;
    while (k < valid_count) {
        double diff = results[k] - mean;
        variance_sum += diff * diff;
        k++;
    }
    
    double std_dev = sqrt(variance_sum / valid_count);
    
    printf("Computation results for %d iterations:\n", valid_count);
    printf("Mean: %.6f\n", mean);
    printf("Standard deviation: %.6f\n", std_dev);
    printf("Minimum: %.6f\n", min_val);
    printf("Maximum: %.6f\n", max_val);
    
    return 0;
}