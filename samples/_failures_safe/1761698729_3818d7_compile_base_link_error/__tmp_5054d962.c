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
    
    double mean = sum / ARRAY_SIZE;
    double geometric_mean;
    
    if (valid_count > 0 && product >= 0) {
        geometric_mean = pow(product, 1.0 / valid_count);
    } else {
        geometric_mean = 0.0;
    }
    
    double variance_sum = 0.0;
    for (int k = 0; k < ARRAY_SIZE; k++) {
        double diff = results[k] - mean;
        variance_sum += diff * diff;
    }
    double variance = variance_sum / ARRAY_SIZE;
    double std_dev = sqrt(variance);
    
    printf("Arithmetic mean: %.6f\n", mean);
    printf("Geometric mean: %.6f\n", geometric_mean);
    printf("Standard deviation: %.6f\n", std_dev);
    
    double running_sum = 0.0;
    printf("Running cumulative sums:\n");
    for (int m = 0; m < ARRAY_SIZE; m++) {
        running_sum += results[m];
        if (m % 20 == 0) {
            printf("Index %d: %.6f\n", m, running_sum);
        }
    }
    
    int convergence_count = 0;
    double prev_value = results[0];
    for (int n = 1; n < ARRAY_SIZE && convergence_count < MAX_ITERATIONS; n++) {
        double ratio = results[n] / prev_value;
        if (fabs(ratio - 1.0) < 0.001) {
            convergence_count++;
        } else {
            convergence_count = 0;
        }
        prev_value = results[n];
    }
    
    printf("Convergence sequence length: %d\n", convergence_count);
    
    return 0;
}