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
    double pi_approximation;
    
    int i = 0;
    while (i < n) {
        double term = 1.0 / (2.0 * i + 1.0);
        if (i % 2 == 0) {
            sum += term;
        } else {
            sum -= term;
        }
        i++;
    }
    
    pi_approximation = 4.0 * sum;
    
    printf("Approximation using %d terms:\n", n);
    
    int j;
    for (j = 0; j < n; j++) {
        if (j < 10 || j >= n - 10) {
            double current_term = 1.0 / (2.0 * j + 1.0);
            int sign = (j % 2 == 0) ? 1 : -1;
            printf("Term %d: %c1/%d\n", j + 1, (sign > 0) ? '+' : '-', 2 * j + 1);
        } else if (j == 10) {
            printf("... (omitting middle terms)\n");
        }
    }
    
    printf("Pi approximation: %.15f\n", pi_approximation);
    
    double error = 3.141592653589793 - pi_approximation;
    if (error < 0) error = -error;
    printf("Absolute error: %.15f\n", error);
    
    int k = 0;
    double running_sum = 0.0;
    do {
        double term = 1.0 / (2.0 * k + 1.0);
        if (k % 2 == 0) {
            running_sum += term;
        } else {
            running_sum -= term;
        }
        k++;
    } while (k < n);
    
    double verification = 4.0 * running_sum;
    printf("Verification sum: %.15f\n", verification);
    
    return 0;
}