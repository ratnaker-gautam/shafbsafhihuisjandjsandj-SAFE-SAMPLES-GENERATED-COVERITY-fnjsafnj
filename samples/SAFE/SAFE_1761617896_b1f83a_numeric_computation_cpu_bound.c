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
    
    if (n > INT_MAX / 1000) {
        fprintf(stderr, "Error: Input too large for safe computation\n");
        return 1;
    }
    
    long double pi_approx = 0.0;
    int iterations = n * 1000;
    
    if (iterations > MAX_ITERATIONS) {
        iterations = MAX_ITERATIONS;
    }
    
    for (int i = 0; i < iterations; i++) {
        long double term = 1.0 / (2 * (long double)i + 1);
        if (i % 2 == 0) {
            pi_approx += term;
        } else {
            pi_approx -= term;
        }
    }
    
    pi_approx *= 4.0;
    
    long double sum = 0.0;
    for (int i = 1; i <= n; i++) {
        sum += (long double)i * (long double)i;
        if (sum > 1e100) {
            fprintf(stderr, "Error: Computation overflow\n");
            return 1;
        }
    }
    
    printf("Pi approximation: %.15Lf\n", pi_approx);
    printf("Sum of squares 1^2 to %d^2: %.0Lf\n", n, sum);
    
    long double harmonic = 0.0;
    for (int i = 1; i <= n; i++) {
        harmonic += 1.0 / (long double)i;
        if (harmonic > 1e100) {
            fprintf(stderr, "Error: Harmonic series overflow\n");
            return 1;
        }
    }
    
    printf("Harmonic number H_%d: %.15Lf\n", n, harmonic);
    
    return 0;
}