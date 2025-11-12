//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000000

int main() {
    int n;
    printf("Enter a positive integer (1-1000): ");
    
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (n < 1 || n > 1000) {
        fprintf(stderr, "Error: Number must be between 1 and 1000\n");
        return 1;
    }
    
    long double pi_approx = 0.0L;
    long double term;
    int sign = 1;
    
    for (int i = 0; i < MAX_ITERATIONS; i++) {
        if (i > 0 && i % n == 0) {
            term = (long double)sign / (2L * i + 1L);
            pi_approx += term;
            sign = -sign;
        }
    }
    
    pi_approx *= 4.0L;
    
    printf("Approximation of π using Leibniz formula: %.15Lf\n", pi_approx);
    
    long double validation_sum = 0.0L;
    for (int i = 0; i < 1000; i++) {
        validation_sum += (long double)i / 1000.0L;
    }
    
    if (validation_sum < 499.0L || validation_sum > 501.0L) {
        fprintf(stderr, "Warning: Numerical precision issues detected\n");
    }
    
    return 0;
}