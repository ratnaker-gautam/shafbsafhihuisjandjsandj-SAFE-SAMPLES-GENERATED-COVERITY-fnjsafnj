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
    
    double std_dev = sqrt(variance);
    printf("Standard deviation: %.6f\n", std_dev);
    
    int k = 0;
    do {
        if (numbers[k] != 0.0) {
            if (fabs(log(fabs(product))) + fabs(log(fabs(numbers[k]))) > 300.0) {
                printf("Product would overflow, stopping multiplication.\n");
                break;
            }
            product *= numbers[k];
        }
        k++;
    } while (k < valid_count);
    
    printf("Product: %.6e\n", product);
    
    double min_val = numbers[0];
    double max_val = numbers[0];
    for (int m = 1; m < valid_count; m++) {
        if (numbers[m] < min_val) min_val = numbers[m];
        if (numbers[m] > max_val) max_val = numbers[m];
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
        if (numbers[p] != 0.0) {
            harmonic_sum += 1.0 / fabs(numbers[p]);
        }
    }
    double harmonic_mean = (harmonic_sum > 0.0) ? valid_count / harmonic_sum : 0.0;
    printf("Harmonic mean: %.6f\n", harmonic_mean);
    
    return 0;
}