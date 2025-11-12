//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
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
        if (scanf("%lf", &input) != 1) {
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
        if (fabs(numbers[j]) < 1e-300) {
            printf("Warning: number too small for safe multiplication\n");
            product = 0.0;
            break;
        }
        double old_product = product;
        product *= numbers[j];
        if (!isfinite(product) || fabs(product) < 1e-300) {
            product = old_product;
            printf("Product calculation overflow/underflow at element %d\n", j);
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
    printf("Minimum: %.6f\n", min_val);
    printf("Maximum: %.6f\n", max_val);
    printf("Product: %.6f\n", product);
    printf("Standard deviation: %.6f\n", std_dev);
    
    printf("\nComputing square roots with iterative refinement:\n");
    for (int idx = 0; idx < valid_count; idx++) {
        if (numbers[idx] < 0) {
            printf("Number %.6f: Cannot compute square root of negative\n", numbers[idx]);
            continue;
        }
        
        double x = numbers[idx] > 0 ? numbers[idx] : 1.0;
        int iter;
        for (iter = 0; iter < MAX_ITERATIONS; iter++) {
            double new_x = 0.5 * (x + numbers[idx] / x);
            if (fabs(new_x - x) < 1e-15 * fabs(new_x)) {
                break;
            }
            x = new_x;
        }
        printf("Number %.6f: sqrt ≈ %.6f (converged in %d iterations)\n", 
               numbers[idx], x, iter + 1);
    }
    
    return 0;
}