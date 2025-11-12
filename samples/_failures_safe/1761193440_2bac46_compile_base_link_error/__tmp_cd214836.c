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
    int i = 0;
    
    printf("Enter up to %d numbers (non-numeric input to stop):\n", ARRAY_SIZE);
    
    while (i < ARRAY_SIZE) {
        double input;
        if (scanf("%lf", &input) != 1) {
            break;
        }
        
        if (!isfinite(input)) {
            printf("Invalid input: non-finite number\n");
            continue;
        }
        
        numbers[i] = input;
        valid_count++;
        i++;
    }
    
    if (valid_count == 0) {
        printf("No valid numbers entered.\n");
        return 1;
    }
    
    min_val = numbers[0];
    max_val = numbers[0];
    
    for (int j = 0; j < valid_count; j++) {
        sum += numbers[j];
        
        if (numbers[j] < min_val) {
            min_val = numbers[j];
        }
        if (numbers[j] > max_val) {
            max_val = numbers[j];
        }
    }
    
    double mean = sum / valid_count;
    
    int k = 0;
    do {
        if (numbers[k] != 0.0) {
            if (fabs(product) > 1e300 / fabs(numbers[k])) {
                product = (product > 0) ? INFINITY : -INFINITY;
                break;
            }
            product *= numbers[k];
        }
        k++;
    } while (k < valid_count);
    
    double variance_sum = 0.0;
    for (int m = 0; m < valid_count; m++) {
        double diff = numbers[m] - mean;
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
    
    printf("\nComputing square roots (iterative method):\n");
    for (int n = 0; n < valid_count; n++) {
        if (numbers[n] < 0) {
            printf("Number %.6f: Cannot compute square root of negative\n", numbers[n]);
            continue;
        }
        
        double x = numbers[n] > 1.0 ? numbers[n] : 1.0;
        int iterations = 0;
        
        while (iterations < MAX_ITERATIONS) {
            double new_x = 0.5 * (x + numbers[n] / x);
            if (fabs(new_x - x) < 1e-10) {
                break;
            }
            x = new_x;
            iterations++;
        }
        
        double sqrt_val = sqrt(numbers[n]);
        printf("Number %.6f: sqrt=%.6f, iterations=%d\n", numbers[n], sqrt_val, iterations);
    }
    
    return 0;
}