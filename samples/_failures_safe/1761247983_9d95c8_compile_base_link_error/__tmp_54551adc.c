//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
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
    
    min_val = numbers[0];
    max_val = numbers[0];
    
    int i = 0;
    while (i < valid_count) {
        sum += numbers[i];
        if (numbers[i] < min_val) min_val = numbers[i];
        if (numbers[i] > max_val) max_val = numbers[i];
        i++;
    }
    
    double mean = sum / valid_count;
    
    double variance_sum = 0.0;
    for (int j = 0; j < valid_count; j++) {
        double diff = numbers[j] - mean;
        variance_sum += diff * diff;
    }
    double variance = variance_sum / valid_count;
    double std_dev = sqrt(variance);
    
    int k = 0;
    do {
        if (numbers[k] != 0.0) {
            if (fabs(product) > 1e308 / fabs(numbers[k])) {
                printf("Warning: Product would overflow\n");
                product = 0.0;
                break;
            }
            product *= numbers[k];
        }
        k++;
    } while (k < valid_count);
    
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
    
    printf("\nComputing square roots with iterative method:\n");
    for (int idx = 0; idx < valid_count; idx++) {
        double x = fabs(numbers[idx]);
        if (x == 0.0) {
            printf("sqrt(0.0) = 0.0\n");
            continue;
        }
        
        double guess = x / 2.0;
        int iter = 0;
        
        while (iter < MAX_ITERATIONS) {
            double new_guess = 0.5 * (guess + x / guess);
            if (fabs(new_guess - guess) < 1e-10) {
                break;
            }
            guess = new_guess;
            iter++;
        }
        
        printf("sqrt(%.6f) ≈ %.6f (iterations: %d)\n", x, guess, iter);
    }
    
    return 0;
}