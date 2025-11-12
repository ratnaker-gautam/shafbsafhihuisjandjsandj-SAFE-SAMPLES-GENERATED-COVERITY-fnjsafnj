//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define ARRAY_SIZE 100

int main() {
    double numbers[ARRAY_SIZE];
    double sum = 0.0;
    double product = 1.0;
    double min_val, max_val;
    int valid_count = 0;
    
    printf("Enter up to %d numbers (non-numeric input to stop):\n", ARRAY_SIZE);
    
    for (int i = 0; i < ARRAY_SIZE; i++) {
        double input;
        if (scanf("%lf", &input) != 1) {
            break;
        }
        
        if (!isfinite(input)) {
            printf("Invalid number: infinity or NaN\n");
            continue;
        }
        
        numbers[valid_count] = input;
        valid_count++;
    }
    
    if (valid_count == 0) {
        printf("No valid numbers entered.\n");
        return 1;
    }
    
    min_val = numbers[0];
    max_val = numbers[0];
    
    int i = 0;
    while (i < valid_count) {
        sum += numbers[i];
        
        if (numbers[i] != 0.0) {
            product *= numbers[i];
        }
        
        if (numbers[i] < min_val) {
            min_val = numbers[i];
        }
        if (numbers[i] > max_val) {
            max_val = numbers[i];
        }
        i++;
    }
    
    double mean = sum / valid_count;
    double variance = 0.0;
    
    int j = 0;
    do {
        double diff = numbers[j] - mean;
        variance += diff * diff;
        j++;
    } while (j < valid_count);
    
    variance /= valid_count;
    double std_dev = sqrt(variance);
    
    double harmonic_sum = 0.0;
    for (int k = 0; k < valid_count; k++) {
        if (numbers[k] != 0.0) {
            harmonic_sum += 1.0 / numbers[k];
        }
    }
    double harmonic_mean = valid_count / harmonic_sum;
    
    double geometric_mean;
    if (product >= 0.0) {
        geometric_mean = pow(product, 1.0 / valid_count);
    } else {
        geometric_mean = -pow(-product, 1.0 / valid_count);
    }
    
    printf("Statistics for %d numbers:\n", valid_count);
    printf("Sum: %.6f\n", sum);
    printf("Mean: %.6f\n", mean);
    printf("Standard deviation: %.6f\n", std_dev);
    printf("Minimum: %.6f\n", min_val);
    printf("Maximum: %.6f\n", max_val);
    printf("Geometric mean: %.6f\n", geometric_mean);
    printf("Harmonic mean: %.6f\n", harmonic_mean);
    
    printf("\nComputing square roots iteratively:\n");
    for (int idx = 0; idx < valid_count; idx++) {
        if (numbers[idx] < 0.0) {
            printf("Number %.6f: Cannot compute square root of negative\n", numbers[idx]);
            continue;
        }
        
        double x = numbers[idx] > 1.0 ? numbers[idx] : 1.0;
        int iter;
        for (iter = 0; iter < MAX_ITERATIONS; iter++) {
            double new_x = 0.5 * (x + numbers[idx] / x);
            if (fabs(new_x - x) < 1e-10) {
                break;
            }
            x = new_x;
        }
        printf("Number %.6f: sqrt ≈ %.6f (converged in %d iterations)\n", 
               numbers[idx], x, iter + 1);
    }
    
    return 0;
}