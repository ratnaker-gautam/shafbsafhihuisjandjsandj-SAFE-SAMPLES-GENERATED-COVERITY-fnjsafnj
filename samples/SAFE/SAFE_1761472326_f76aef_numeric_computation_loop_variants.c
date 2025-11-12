//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000

int main(void) {
    int n;
    printf("Enter number of terms to compute (1-%d): ", MAX_ITERATIONS);
    
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (n < 1 || n > MAX_ITERATIONS) {
        fprintf(stderr, "Error: Number out of range\n");
        return 1;
    }
    
    double sum = 0.0;
    double term;
    int i = 0;
    
    printf("Computing series approximation...\n");
    
    while (i < n) {
        term = 1.0 / ((2.0 * i) + 1.0);
        if (i % 2 == 0) {
            sum += term;
        } else {
            sum -= term;
        }
        i++;
    }
    
    double pi_approx = 4.0 * sum;
    
    printf("Approximation using %d terms:\n", n);
    printf("π ≈ %.15f\n", pi_approx);
    
    double error_bound = 4.0 / ((2.0 * n) + 1.0);
    printf("Error bound: < %.15f\n", error_bound);
    
    double cumulative = 0.0;
    printf("Partial sums:\n");
    for (int j = 0; j < n; j++) {
        term = 1.0 / ((2.0 * j) + 1.0);
        if (j % 2 == 0) {
            cumulative += term;
        } else {
            cumulative -= term;
        }
        if (j < 10 || j == n - 1) {
            printf("Term %d: %.10f\n", j + 1, 4.0 * cumulative);
        }
    }
    
    double running_sum = 0.0;
    int k = 0;
    printf("Running average of approximations:\n");
    do {
        term = 1.0 / ((2.0 * k) + 1.0);
        if (k % 2 == 0) {
            running_sum += term;
        } else {
            running_sum -= term;
        }
        double current_approx = 4.0 * running_sum;
        double avg_approx = current_approx / (k + 1);
        if (k < 5 || k == n - 1) {
            printf("After %d terms: average = %.10f\n", k + 1, avg_approx);
        }
        k++;
    } while (k < n);
    
    return 0;
}