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
            return 1;
        }
        
        numbers[valid_count] = input;
        valid_count++;
    }
    
    if (valid_count == 0) {
        printf("No valid numbers entered\n");
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
    
    double temp_product = 1.0;
    for (int j = 0; j < valid_count; j++) {
        if (numbers[j] != 0.0) {
            if (fabs(log(fabs(temp_product)) + log(fabs(numbers[j]))) > 300.0) {
                printf("Product would overflow\n");
                temp_product = 0.0;
                break;
            }
            temp_product *= numbers[j];
        }
    }
    product = temp_product;
    
    double mean = sum / valid_count;
    
    double variance_sum = 0.0;
    int k = 0;
    do {
        double diff = numbers[k] - mean;
        variance_sum += diff * diff;
        k++;
    } while (k < valid_count);
    
    double variance = variance_sum / valid_count;
    double std_dev = sqrt(variance);
    
    printf("Statistics for %d numbers:\n", valid_count);
    printf("Sum: %.6f\n", sum);
    printf("Product: %.6f\n", product);
    printf("Mean: %.6f\n", mean);
    printf("Min: %.6f\n", min_val);
    printf("Max: %.6f\n", max_val);
    printf("Standard deviation: %.6f\n", std_dev);
    
    printf("Computing square roots with iterative refinement:\n");
    for (int m = 0; m < valid_count; m++) {
        if (numbers[m] < 0) {
            printf("Number %.6f: Cannot compute real square root\n", numbers[m]);
            continue;
        }
        
        double x = numbers[m] > 0 ? numbers[m] / 2.0 : 0.0;
        int iter;
        for (iter = 0; iter < MAX_ITERATIONS; iter++) {
            double prev_x = x;
            if (x != 0.0) {
                x = 0.5 * (x + numbers[m] / x);
            }
            if (fabs(x - prev_x) < 1e-15) {
                break;
            }
        }
        printf("Number %.6f: sqrt ≈ %.6f (converged in %d iterations)\n", 
               numbers[m], x, iter + 1);
    }
    
    return 0;
}