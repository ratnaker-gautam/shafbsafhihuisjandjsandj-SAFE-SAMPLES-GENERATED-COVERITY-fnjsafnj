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
    int iterations = (n > MAX_ITERATIONS) ? MAX_ITERATIONS : n;
    
    for (int i = 0; i < iterations; i++) {
        long double term = 1.0L / (2 * i + 1);
        if (i % 2 == 0) {
            pi_approx += term;
        } else {
            pi_approx -= term;
        }
    }
    
    pi_approx *= 4.0L;
    
    printf("Approximation of π using %d terms: %.15Lf\n", iterations, pi_approx);
    
    long double sum = 0.0L;
    for (int i = 1; i <= iterations; i++) {
        sum += 1.0L / (i * i);
    }
    
    long double basel_approx = 0.0L;
    if (sum > 0.0L) {
        basel_approx = 1.0L / sum;
        basel_approx = 6.0L * basel_approx;
    }
    
    printf("Approximation of π²/6 using %d terms: %.15Lf\n", iterations, sum);
    printf("Approximation of π using Basel problem: %.15Lf\n", basel_approx);
    
    return 0;
}