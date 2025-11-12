//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define ARRAY_SIZE 100

int main() {
    double numbers[ARRAY_SIZE];
    int valid_count = 0;
    double sum = 0.0;
    double product = 1.0;
    double min_val = 1.0 / 0.0;
    double max_val = -1.0 / 0.0;
    
    printf("Enter up to %d numbers (non-numeric input to stop):\n", ARRAY_SIZE);
    
    for (int i = 0; i < ARRAY_SIZE; i++) {
        double input;
        int result = scanf("%lf", &input);
        
        if (result != 1) {
            break;
        }
        
        if (!isfinite(input)) {
            printf("Invalid input: non-finite number\n");
            continue;
        }
        
        numbers[valid_count] = input;
        valid_count++;
    }
    
    if (valid_count == 0) {
        printf("No valid numbers entered.\n");
        return 1;
    }
    
    int i = 0;
    while (i < valid_count) {
        sum += numbers[i];
        i++;
    }
    
    double mean = sum / valid_count;
    
    i = 0;
    do {
        if (numbers[i] < min_val) {
            min_val = numbers[i];
        }
        if (numbers[i] > max_val) {
            max_val = numbers[i];
        }
        i++;
    } while (i < valid_count);
    
    for (int j = 0; j < valid_count; j++) {
        if (numbers[j] == 0.0) {
            product = 0.0;
            break;
        }
        
        double old_product = product;
        product *= numbers[j];
        
        if (!isfinite(product) || (numbers[j] != 0.0 && product / numbers[j] != old_product)) {
            product = old_product;
            printf("Product calculation overflow at element %d\n", j);
            break;
        }
    }
    
    double variance_sum = 0.0;
    for (int k = 0; k < valid_count; k++) {
        double diff = numbers[k] - mean;
        variance_sum += diff * diff;
    }
    double variance = variance_sum / valid_count;
    double std_dev = sqrt(variance);
    
    printf("Statistics for %d numbers:\n", valid_count);
    printf("Sum: %.6f\n", sum);
    printf("Mean: %.6f\n", mean);
    printf("Product: %.6f\n", product);
    printf("Minimum: %.6f\n", min_val);
    printf("Maximum: %.6f\n", max_val);
    printf("Standard deviation: %.6f\n", std_dev);
    
    printf("\nRunning iterative mean refinement:\n");
    double refined_mean = 0.0;
    for (int iter = 0; iter < MAX_ITERATIONS; iter++) {
        double new_mean = 0.0;
        for (int idx = 0; idx < valid_count; idx++) {
            new_mean += numbers[idx];
        }
        new_mean /= valid_count;
        
        if (fabs(new_mean - refined_mean) < 1e-12) {
            break;
        }
        refined_mean = new_mean;
    }
    printf("Refined mean after convergence: %.12f\n", refined_mean);
    
    return 0;
}