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
    
    int j = 0;
    do {
        if (numbers[j] != 0.0) {
            product *= numbers[j];
        }
        j++;
    } while (j < valid_count);
    
    for (int k = 0; k < valid_count; k++) {
        if (numbers[k] < min_val) {
            min_val = numbers[k];
        }
        if (numbers[k] > max_val) {
            max_val = numbers[k];
        }
    }
    
    double variance_sum = 0.0;
    int m = 0;
    while (m < valid_count) {
        double diff = numbers[m] - mean;
        variance_sum += diff * diff;
        m++;
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
    
    printf("\nComputing square roots with iterative refinement:\n");
    for (int n = 0; n < valid_count && n < 10; n++) {
        double x = fabs(numbers[n]);
        if (x == 0.0) {
            printf("sqrt(0.000000) = 0.000000\n");
            continue;
        }
        
        double guess = x / 2.0;
        int iterations = 0;
        
        while (iterations < MAX_ITERATIONS) {
            double new_guess = 0.5 * (guess + x / guess);
            if (fabs(new_guess - guess) < 1e-10) {
                break;
            }
            guess = new_guess;
            iterations++;
        }
        
        double actual_sqrt = sqrt(x);
        printf("sqrt(%.6f) = %.6f (iterations: %d)\n", x, guess, iterations);
    }
    
    return 0;
}