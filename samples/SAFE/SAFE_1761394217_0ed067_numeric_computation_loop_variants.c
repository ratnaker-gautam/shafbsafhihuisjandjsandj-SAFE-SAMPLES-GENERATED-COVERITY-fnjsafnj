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
        fprintf(stderr, "Error: Number must be between 1 and %d\n", MAX_ITERATIONS);
        return 1;
    }
    
    double sum = 0.0;
    double term;
    int i = 0;
    
    printf("Computing alternating harmonic series for %d terms:\n", n);
    
    while (i < n) {
        term = 1.0 / (i + 1);
        if (i % 2 == 1) {
            term = -term;
        }
        sum += term;
        printf("Term %d: %f, Partial sum: %f\n", i + 1, term, sum);
        i++;
    }
    
    printf("\n");
    printf("Final sum after %d terms: %f\n", n, sum);
    
    double pi_approx = sum * 4;
    printf("Approximation of pi (sum * 4): %f\n", pi_approx);
    
    double error = 3.14159265358979323846 - pi_approx;
    printf("Error from actual pi: %f\n", error);
    
    double running_sum = 0.0;
    printf("\nVerification with for loop:\n");
    for (int j = 0; j < n; j++) {
        double verify_term = 1.0 / (j + 1);
        if (j % 2 == 1) {
            verify_term = -verify_term;
        }
        running_sum += verify_term;
    }
    printf("Verified sum: %f\n", running_sum);
    
    double diff = sum - running_sum;
    if (diff < 0) diff = -diff;
    if (diff > 1e-10) {
        fprintf(stderr, "Error: Verification failed\n");
        return 1;
    }
    
    printf("Verification successful\n");
    
    return 0;
}