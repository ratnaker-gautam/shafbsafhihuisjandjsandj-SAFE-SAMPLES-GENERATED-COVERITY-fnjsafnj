//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <float.h>

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
    
    long double pi_approx = 0.0L;
    int iterations = n * 1000;
    
    if (iterations > MAX_ITERATIONS) {
        iterations = MAX_ITERATIONS;
    }
    
    if (iterations < 0) {
        fprintf(stderr, "Error: Iteration count overflow\n");
        return 1;
    }
    
    for (int i = 0; i < iterations; i++) {
        if (i > INT_MAX / 2 - 1) {
            break;
        }
        long double term = 1.0L / (2 * i + 1);
        if (i % 2 == 0) {
            pi_approx += term;
        } else {
            pi_approx -= term;
        }
    }
    
    pi_approx *= 4.0L;
    
    long double e_approx = 1.0L;
    long double factorial = 1.0L;
    
    for (int i = 1; i <= n; i++) {
        if (factorial > LDBL_MAX / i) {
            break;
        }
        factorial *= i;
        e_approx += 1.0L / factorial;
    }
    
    long double result = pi_approx * e_approx;
    
    printf("Computation completed with %d iterations\n", iterations);
    printf("Approximate result: %.15Lf\n", result);
    
    return 0;
}