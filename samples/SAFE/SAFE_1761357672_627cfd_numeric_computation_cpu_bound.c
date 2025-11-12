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
    
    for (int i = 0; i < MAX_ITERATIONS; i++) {
        if (i > INT_MAX / 2) {
            break;
        }
        
        long double term = 1.0L / (2 * i + 1);
        if (sign > 0) {
            pi_approx += term;
        } else {
            pi_approx -= term;
        }
        
        sign = -sign;
        
        if (i % n == 0 && i > 0) {
            printf("After %d iterations: pi ≈ %.15Lf\n", i, 4.0L * pi_approx);
        }
    }
    
    pi_approx *= 4.0L;
    printf("Final approximation: pi ≈ %.15Lf\n", pi_approx);
    
    long double error = 3.14159265358979323846L - pi_approx;
    if (error < 0) {
        error = -error;
    }
    printf("Absolute error: %.15Lf\n", error);
    
    return 0;
}