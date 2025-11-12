//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define ARRAY_SIZE 100

int main() {
    double results[ARRAY_SIZE];
    double sum = 0.0;
    double product = 1.0;
    int valid_count = 0;
    
    printf("Enter %d numbers between -1000 and 1000:\n", ARRAY_SIZE);
    
    for (int i = 0; i < ARRAY_SIZE; i++) {
        double input;
        if (scanf("%lf", &input) != 1) {
            printf("Invalid input. Please enter numbers only.\n");
            return 1;
        }
        
        if (input < -1000.0 || input > 1000.0) {
            printf("Number out of range. Must be between -1000 and 1000.\n");
            return 1;
        }
        
        results[i] = input;
    }
    
    int i = 0;
    while (i < ARRAY_SIZE) {
        sum += results[i];
        i++;
    }
    
    int j = 0;
    do {
        if (fabs(results[j]) > 1e-10) {
            product *= results[j];
            valid_count++;
        }
        j++;
    } while (j < ARRAY_SIZE);
    
    double average = sum / ARRAY_SIZE;
    double geometric_mean;
    
    if (valid_count > 0 && product >= 0) {
        geometric_mean = pow(product, 1.0 / valid_count);
    } else {
        geometric_mean = 0.0;
    }
    
    double min_val = results[0];
    double max_val = results[0];
    
    for (int k = 1; k < ARRAY_SIZE; k++) {
        if (results[k] < min_val) {
            min_val = results[k];
        }
        if (results[k] > max_val) {
            max_val = results[k];
        }
    }
    
    double variance_sum = 0.0;
    int m = 0;
    while (m < ARRAY_SIZE) {
        double diff = results[m] - average;
        variance_sum += diff * diff;
        m++;
    }
    double variance = variance_sum / ARRAY_SIZE;
    double std_dev = sqrt(variance);
    
    printf("Statistical Analysis Results:\n");
    printf("Sum: %.6f\n", sum);
    printf("Average: %.6f\n", average);
    printf("Geometric Mean: %.6f\n", geometric_mean);
    printf("Minimum: %.6f\n", min_val);
    printf("Maximum: %.6f\n", max_val);
    printf("Standard Deviation: %.6f\n", std_dev);
    
    int count = 0;
    for (int n = 0; n < ARRAY_SIZE; n++) {
        if (results[n] > average + std_dev || results[n] < average - std_dev) {
            count++;
        }
    }
    printf("Outliers (beyond 1 std dev): %d\n", count);
    
    double running_sum = 0.0;
    printf("Running averages:\n");
    for (int p = 0; p < ARRAY_SIZE; p++) {
        running_sum += results[p];
        printf("After %d elements: %.6f\n", p + 1, running_sum / (p + 1));
    }
    
    return 0;
}