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
    
    printf("Enter up to %d numbers (non-numeric input to stop):\n", ARRAY_SIZE);
    
    for (int i = 0; i < ARRAY_SIZE; i++) {
        double input;
        printf("Number %d: ", i + 1);
        
        if (scanf("%lf", &input) != 1) {
            break;
        }
        
        if (!isfinite(input)) {
            printf("Invalid number (infinity/NaN). Stopping.\n");
            break;
        }
        
        numbers[valid_count] = input;
        valid_count++;
    }
    
    if (valid_count == 0) {
        printf("No valid numbers entered.\n");
        return 1;
    }
    
    printf("\nProcessing %d numbers...\n", valid_count);
    
    int i = 0;
    while (i < valid_count) {
        sum += numbers[i];
        i++;
    }
    
    double mean = sum / valid_count;
    printf("Mean: %.6f\n", mean);
    
    double variance_sum = 0.0;
    for (int j = 0; j < valid_count; j++) {
        double diff = numbers[j] - mean;
        variance_sum += diff * diff;
    }
    double variance = variance_sum / valid_count;
    printf("Variance: %.6f\n", variance);
    
    double geometric_mean = 1.0;
    int k = 0;
    do {
        if (numbers[k] <= 0.0) {
            printf("Cannot compute geometric mean with non-positive numbers.\n");
            geometric_mean = -1.0;
            break;
        }
        geometric_mean *= pow(numbers[k], 1.0 / valid_count);
        k++;
    } while (k < valid_count);
    
    if (geometric_mean > 0.0) {
        printf("Geometric mean: %.6f\n", geometric_mean);
    }
    
    double min_val = numbers[0];
    double max_val = numbers[0];
    for (int m = 1; m < valid_count; m++) {
        if (numbers[m] < min_val) {
            min_val = numbers[m];
        }
        if (numbers[m] > max_val) {
            max_val = numbers[m];
        }
    }
    printf("Range: [%.6f, %.6f]\n", min_val, max_val);
    
    int count_above_mean = 0;
    int n = 0;
    while (n < valid_count) {
        if (numbers[n] > mean) {
            count_above_mean++;
        }
        n++;
    }
    printf("Numbers above mean: %d\n", count_above_mean);
    
    double harmonic_sum = 0.0;
    for (int p = 0; p < valid_count; p++) {
        if (numbers[p] == 0.0) {
            printf("Cannot compute harmonic mean with zero values.\n");
            harmonic_sum = -1.0;
            break;
        }
        harmonic_sum += 1.0 / numbers[p];
    }
    
    if (harmonic_sum > 0.0) {
        double harmonic_mean = valid_count / harmonic_sum;
        printf("Harmonic mean: %.6f\n", harmonic_mean);
    }
    
    return 0;
}