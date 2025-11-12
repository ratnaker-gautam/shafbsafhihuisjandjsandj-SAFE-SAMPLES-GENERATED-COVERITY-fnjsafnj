//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define ARRAY_SIZE 100

int main() {
    double numbers[ARRAY_SIZE];
    int n;
    
    printf("Enter number of elements (1-%d): ", ARRAY_SIZE);
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (n < 1 || n > ARRAY_SIZE) {
        fprintf(stderr, "Error: Number of elements must be between 1 and %d\n", ARRAY_SIZE);
        return 1;
    }
    
    printf("Enter %d numbers:\n", n);
    for (int i = 0; i < n; i++) {
        if (scanf("%lf", &numbers[i]) != 1) {
            fprintf(stderr, "Error: Invalid number input\n");
            return 1;
        }
    }
    
    double sum = 0.0;
    int i = 0;
    while (i < n) {
        sum += numbers[i];
        i++;
    }
    
    double mean = sum / n;
    
    double variance = 0.0;
    i = 0;
    do {
        double diff = numbers[i] - mean;
        variance += diff * diff;
        i++;
    } while (i < n);
    variance /= n;
    
    double std_dev = sqrt(variance);
    
    double min_val = numbers[0];
    double max_val = numbers[0];
    for (int j = 1; j < n; j++) {
        if (numbers[j] < min_val) {
            min_val = numbers[j];
        }
        if (numbers[j] > max_val) {
            max_val = numbers[j];
        }
    }
    
    double running_sum = 0.0;
    double moving_avg[ARRAY_SIZE];
    for (int k = 0; k < n; k++) {
        running_sum += numbers[k];
        moving_avg[k] = running_sum / (k + 1);
    }
    
    double approx_pi = 0.0;
    int iter_count = 0;
    for (int m = 0; m < MAX_ITERATIONS; m++) {
        double term = 1.0 / (2.0 * m + 1.0);
        if (m % 2 == 0) {
            approx_pi += term;
        } else {
            approx_pi -= term;
        }
        iter_count++;
        
        if (fabs(approx_pi * 4.0 - 3.14159) < 0.0001) {
            break;
        }
    }
    approx_pi *= 4.0;
    
    printf("\nStatistical Analysis Results:\n");
    printf("Count: %d\n", n);
    printf("Mean: %.6f\n", mean);
    printf("Standard Deviation: %.6f\n", std_dev);
    printf("Minimum: %.6f\n", min_val);
    printf("Maximum: %.6f\n", max_val);
    printf("\nPi Approximation: %.6f (converged in %d iterations)\n", approx_pi, iter_count);
    
    printf("\nMoving Averages:\n");
    for (int idx = 0; idx < n; idx++) {
        printf("Element %d: %.6f (value: %.6f)\n", idx + 1, moving_avg[idx], numbers[idx]);
    }
    
    return 0;
}