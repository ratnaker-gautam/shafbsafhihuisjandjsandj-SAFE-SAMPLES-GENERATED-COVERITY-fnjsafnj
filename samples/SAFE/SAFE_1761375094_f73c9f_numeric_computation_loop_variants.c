//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define ARRAY_SIZE 100

int main() {
    double results[ARRAY_SIZE];
    int n;
    
    printf("Enter number of iterations (1-%d): ", ARRAY_SIZE);
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (n < 1 || n > ARRAY_SIZE) {
        fprintf(stderr, "Error: Number out of range\n");
        return 1;
    }
    
    double sum = 0.0;
    int i = 0;
    
    while (i < n) {
        double term = 1.0 / ((2.0 * i) + 1.0);
        if (i % 2 == 0) {
            results[i] = term;
        } else {
            results[i] = -term;
        }
        sum += results[i];
        i++;
    }
    
    double pi_approx = 4.0 * sum;
    
    printf("Approximation of pi using %d terms: %.10f\n", n, pi_approx);
    
    double running_sum = 0.0;
    double running_avg[ARRAY_SIZE];
    
    for (int j = 0; j < n; j++) {
        running_sum += results[j];
        running_avg[j] = running_sum / (j + 1);
    }
    
    printf("Running averages:\n");
    int k = 0;
    do {
        if (k < n) {
            printf("Term %d: %f\n", k + 1, running_avg[k]);
        }
        k++;
    } while (k < n);
    
    double min_val = results[0];
    double max_val = results[0];
    
    for (int idx = 1; idx < n; idx++) {
        if (results[idx] < min_val) {
            min_val = results[idx];
        }
        if (results[idx] > max_val) {
            max_val = results[idx];
        }
    }
    
    printf("Minimum term: %f\n", min_val);
    printf("Maximum term: %f\n", max_val);
    
    double variance_sum = 0.0;
    double mean = sum / n;
    
    for (int m = 0; m < n; m++) {
        double diff = results[m] - mean;
        variance_sum += diff * diff;
    }
    
    double variance = variance_sum / n;
    printf("Variance of terms: %f\n", variance);
    
    return 0;
}