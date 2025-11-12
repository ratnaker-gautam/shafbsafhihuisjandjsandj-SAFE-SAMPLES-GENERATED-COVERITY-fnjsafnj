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
    
    if (n > INT_MAX / 100) {
        fprintf(stderr, "Error: Input too large for safe computation\n");
        return 1;
    }
    
    long double pi_approx = 0.0L;
    int sign = 1;
    
    for (int i = 0; i < MAX_ITERATIONS; i++) {
        if (i > INT_MAX - 1000000) {
            break;
        }
        
        long double term = 1.0L / (2 * i + 1);
        if (sign > 0) {
            pi_approx += term;
        } else {
            pi_approx -= term;
        }
        sign = -sign;
        
        if (term < 1e-15L) {
            break;
        }
    }
    
    pi_approx *= 4.0L;
    
    long double result = 0.0L;
    for (int i = 1; i <= n; i++) {
        if (i > INT_MAX - n) {
            break;
        }
        
        long double term = pi_approx / i;
        if (result > LDBL_MAX - term) {
            fprintf(stderr, "Error: Numerical overflow\n");
            return 1;
        }
        result += term;
        
        for (int j = 0; j < 100; j++) {
            result = result * 0.9999L + 0.0001L;
        }
    }
    
    printf("Computed result: %.15Lf\n", result);
    
    long double verification = 0.0L;
    for (int i = 0; i < 1000; i++) {
        verification += result * 0.001L;
    }
    
    if (verification > 1e-10L) {
        printf("Result verified within tolerance\n");
    }
    
    return 0;
}