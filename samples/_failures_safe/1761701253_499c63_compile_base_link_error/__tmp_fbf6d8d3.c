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
    
    int j = 0;
    while (j < ARRAY_SIZE) {
        sum += results[j];
        j++;
    }
    
    int k = 0;
    do {
        if (results[k] != 0.0) {
            if (fabs(product) > 1e300 / fabs(results[k])) {
                printf("Product would overflow. Stopping calculation.\n");
                break;
            }
            product *= results[k];
            valid_count++;
        }
        k++;
    } while (k < ARRAY_SIZE);
    
    double average = sum / ARRAY_SIZE;
    double geometric_mean;
    
    if (valid_count > 0 && product >= 0.0) {
        geometric_mean = pow(product, 1.0 / valid_count);
    } else {
        geometric_mean = 0.0;
    }
    
    double variance_sum = 0.0;
    for (int m = 0; m < ARRAY_SIZE; m++) {
        double diff = results[m] - average;
        variance_sum += diff * diff;
    }
    double variance = variance_sum / ARRAY_SIZE;
    double std_dev = sqrt(variance);
    
    printf("Statistical Analysis Results:\n");
    printf("Sum: %.6f\n", sum);
    printf("Average: %.6f\n", average);
    printf("Geometric Mean: %.6f\n", geometric_mean);
    printf("Standard Deviation: %.6f\n", std_dev);
    printf("Valid numbers for product: %d\n", valid_count);
    
    double min_val = results[0];
    double max_val = results[0];
    for (int n = 1; n < ARRAY_SIZE; n++) {
        if (results[n] < min_val) min_val = results[n];
        if (results[n] > max_val) max_val = results[n];
    }
    
    printf("Minimum value: %.6f\n", min_val);
    printf("Maximum value: %.6f\n", max_val);
    
    return 0;
}