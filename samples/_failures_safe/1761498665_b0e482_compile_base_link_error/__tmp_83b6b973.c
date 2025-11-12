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
            printf("Invalid input. Please enter numeric values only.\n");
            return 1;
        }
        
        if (input < -1000.0 || input > 1000.0) {
            printf("Value out of range [-1000, 1000].\n");
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
        if (results[j] != 0.0) {
            if (fabs(product) > 1e300 / fabs(results[j])) {
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
    
    for (int m = 1; m < ARRAY_SIZE; m++) {
        if (results[m] < min_val) {
            min_val = results[m];
        }
        if (results[m] > max_val) {
            max_val = results[m];
        }
    }
    
    printf("Statistical Analysis Results:\n");
    printf("Sum: %.6f\n", sum);
    printf("Product: %.6f\n", product);
    printf("Mean: %.6f\n", mean);
    printf("Variance: %.6f\n", variance);
    printf("Standard Deviation: %.6f\n", std_dev);
    printf("Minimum: %.6f\n", min_val);
    printf("Maximum: %.6f\n", max_val);
    
    int count = 0;
    for (int n = 0; n < ARRAY_SIZE; n++) {
        if (results[n] > mean - std_dev && results[n] < mean + std_dev) {
            count++;
        }
    }
    
    printf("Values within one standard deviation of mean: %d\n", count);
    
    return 0;
}