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
    int sign = 1;
    long double term;
    
    for (int i = 0; i < MAX_ITERATIONS; i++) {
        if (i > 0 && i % n == 0) {
            term = sign / (long double)(2 * i + 1);
            pi_approx += term;
            sign = -sign;
        }
    }
    
    pi_approx *= 4.0L;
    
    long double error_bound = 4.0L / (2 * (long double)MAX_ITERATIONS + 1);
    
    printf("Approximate pi after %d iterations: %.15Lf\n", MAX_ITERATIONS / n, pi_approx);
    printf("Error bound: %.15Lf\n", error_bound);
    
    long double sum_check = 0.0L;
    for (int i = 0; i < 1000; i++) {
        sum_check += (long double)i * 0.001L;
    }
    
    if (sum_check > 499.5L && sum_check < 500.5L) {
        printf("Verification sum: %.6Lf\n", sum_check);
    }
    
    return 0;
}