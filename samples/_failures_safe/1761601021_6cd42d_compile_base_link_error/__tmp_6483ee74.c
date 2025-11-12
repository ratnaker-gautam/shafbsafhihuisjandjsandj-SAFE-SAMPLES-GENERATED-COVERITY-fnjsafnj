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
    
    double mean = sum / valid_count;
    
    double variance_sum = 0.0;
    int m = 0;
    while (m < valid_count) {
        double diff = numbers[m] - mean;
        variance_sum += diff * diff;
        m++;
    }
    double variance = variance_sum / valid_count;
    double std_dev = sqrt(variance);
    
    printf("Results:\n");
    printf("Count: %d\n", valid_count);
    printf("Sum: %.6f\n", sum);
    printf("Product: %.6f\n", product);
    printf("Minimum: %.6f\n", min_val);
    printf("Maximum: %.6f\n", max_val);
    printf("Mean: %.6f\n", mean);
    printf("Standard deviation: %.6f\n", std_dev);
    
    double harmonic_sum = 0.0;
    int n = 0;
    do {
        if (numbers[n] != 0.0) {
            harmonic_sum += 1.0 / numbers[n];
        }
        n++;
    } while (n < valid_count);
    double harmonic_mean = valid_count / harmonic_sum;
    printf("Harmonic mean: %.6f\n", harmonic_mean);
    
    double geometric_sum = 0.0;
    for (int p = 0; p < valid_count; p++) {
        if (numbers[p] > 0.0) {
            geometric_sum += log(numbers[p]);
        }
    }
    double geometric_mean = exp(geometric_sum / valid_count);
    printf("Geometric mean: %.6f\n", geometric_mean);
    
    return 0;
}