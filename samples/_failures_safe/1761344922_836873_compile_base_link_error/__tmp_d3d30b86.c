//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
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
    
    printf("Enter %d numbers between -1000.0 and 1000.0:\n", ARRAY_SIZE);
    
    for (int i = 0; i < ARRAY_SIZE; i++) {
        double input;
        if (scanf("%lf", &input) != 1) {
            printf("Invalid input. Please enter numbers only.\n");
            return 1;
        }
        
        if (input < -1000.0 || input > 1000.0) {
            printf("Number out of range. Must be between -1000.0 and 1000.0.\n");
            return 1;
        }
        
        results[i] = input;
        valid_count++;
    }
    
    if (valid_count != ARRAY_SIZE) {
        printf("Insufficient valid inputs.\n");
        return 1;
    }
    
    int i = 0;
    while (i < ARRAY_SIZE) {
        sum += results[i];
        i++;
    }
    
    int j = 0;
    do {
        if (fabs(results[j]) > 1e-10) {
            if (product > 1e308 / fabs(results[j])) {
                printf("Product would overflow.\n");
                return 1;
            }
            product *= results[j];
        }
        j++;
    } while (j < ARRAY_SIZE);
    
    double mean = sum / ARRAY_SIZE;
    double variance_sum = 0.0;
    
    for (int k = 0; k < ARRAY_SIZE; k++) {
        double diff = results[k] - mean;
        variance_sum += diff * diff;
    }
    
    double variance = variance_sum / ARRAY_SIZE;
    double std_dev = sqrt(variance);
    
    double min_val = results[0];
    double max_val = results[0];
    
    int idx = 0;
    while (idx < ARRAY_SIZE) {
        if (results[idx] < min_val) {
            min_val = results[idx];
        }
        if (results[idx] > max_val) {
            max_val = results[idx];
        }
        idx++;
    }
    
    printf("Statistical Analysis Results:\n");
    printf("Sum: %.6f\n", sum);
    printf("Product: %.6f\n", product);
    printf("Mean: %.6f\n", mean);
    printf("Variance: %.6f\n", variance);
    printf("Standard Deviation: %.6f\n", std_dev);
    printf("Minimum: %.6f\n", min_val);
    printf("Maximum: %.6f\n", max_val);
    
    double running_sum = 0.0;
    printf("\nRunning cumulative sums:\n");
    for (int m = 0; m < ARRAY_SIZE; m++) {
        running_sum += results[m];
        printf("After element %d: %.6f\n", m + 1, running_sum);
    }
    
    return 0;
}