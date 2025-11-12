//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <float.h>

#define MAX_ITERATIONS 1000
#define ARRAY_SIZE 100

int main() {
    int n;
    printf("Enter number of iterations (1-%d): ", MAX_ITERATIONS);
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (n < 1 || n > MAX_ITERATIONS) {
        fprintf(stderr, "Error: Number out of range\n");
        return 1;
    }

    double results[ARRAY_SIZE] = {0};
    double sum = 0.0;
    double product = 1.0;
    
    int i = 0;
    while (i < n && i < ARRAY_SIZE) {
        double term = 1.0 / (i + 1.0);
        results[i] = term;
        sum += term;
        
        if (term > 0.0 && product > DBL_MAX / term / 100.0) {
            product = 1.0;
        }
        product *= term;
        
        i++;
    }

    double harmonic_mean = 0.0;
    if (n > 0 && sum != 0.0) {
        harmonic_mean = (double)n / sum;
    }

    printf("Results for %d iterations:\n", n);
    printf("Sum of reciprocals: %.10f\n", sum);
    printf("Harmonic mean: %.10f\n", harmonic_mean);
    printf("Partial product: %.10f\n", product);

    double running_sum = 0.0;
    for (int j = 0; j < n && j < ARRAY_SIZE; j++) {
        running_sum += results[j];
        printf("Term %d: %.6f (cumulative: %.6f)\n", j + 1, results[j], running_sum);
    }

    double alt_sum = 0.0;
    int k = 0;
    do {
        if (k < n && k < ARRAY_SIZE) {
            alt_sum += results[k];
        }
        k++;
    } while (k < n && k < ARRAY_SIZE);

    printf("Verification sum: %.10f\n", alt_sum);

    double max_val = 0.0;
    double min_val = 1.0;
    for (int idx = 0; idx < n && idx < ARRAY_SIZE; idx++) {
        if (results[idx] > max_val) {
            max_val = results[idx];
        }
        if (results[idx] < min_val) {
            min_val = results[idx];
        }
    }

    printf("Maximum term: %.6f\n", max_val);
    printf("Minimum term: %.6f\n", min_val);

    return 0;
}