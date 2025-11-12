//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000

int main() {
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
    
    printf("Computing series approximation of pi/4 using %d terms\n", n);
    
    while (i < n) {
        term = 1.0 / (2 * i + 1);
        if (i % 2 == 1) {
            term = -term;
        }
        sum += term;
        i++;
    }
    
    double pi_approx = sum * 4.0;
    printf("Approximation of pi: %.15f\n", pi_approx);
    
    double running_sum = 0.0;
    double running_term;
    printf("Verifying with for loop...\n");
    
    for (int j = 0; j < n; j++) {
        running_term = 1.0 / (2 * j + 1);
        if (j % 2 == 1) {
            running_term = -running_term;
        }
        running_sum += running_term;
    }
    
    double pi_verify = running_sum * 4.0;
    printf("Verification result: %.15f\n", pi_verify);
    
    double diff = pi_approx - pi_verify;
    if (diff < 0) diff = -diff;
    
    printf("Difference between methods: %e\n", diff);
    
    int k = 0;
    double partial_sum = 0.0;
    printf("Partial sums every 100 terms:\n");
    
    do {
        if (k >= n) break;
        
        double partial_term = 1.0 / (2 * k + 1);
        if (k % 2 == 1) {
            partial_term = -partial_term;
        }
        partial_sum += partial_term;
        
        if ((k + 1) % 100 == 0 || k == n - 1) {
            printf("Term %d: partial pi = %.10f\n", k + 1, partial_sum * 4.0);
        }
        
        k++;
    } while (k < n);
    
    return 0;
}