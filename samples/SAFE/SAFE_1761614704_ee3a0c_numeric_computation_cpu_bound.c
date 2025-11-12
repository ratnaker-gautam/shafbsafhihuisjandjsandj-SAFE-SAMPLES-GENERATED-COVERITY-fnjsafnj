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
    
    long double pi_approx = 0.0L;
    int sign = 1;
    long double term;
    int actual_iterations = 0;
    
    for (int i = 0; i < MAX_ITERATIONS; i++) {
        if (i >= n) {
            break;
        }
        
        long double denominator = 2.0L * i + 1.0L;
        if (denominator == 0.0L) {
            continue;
        }
        
        term = 1.0L / denominator;
        if (sign > 0) {
            if (pi_approx > LDBL_MAX - term) {
                break;
            }
            pi_approx += term;
        } else {
            if (pi_approx < -LDBL_MAX + term) {
                break;
            }
            pi_approx -= term;
        }
        
        sign = -sign;
        actual_iterations++;
    }
    
    pi_approx *= 4.0L;
    
    printf("Approximation of π using %d terms: %.15Lf\n", actual_iterations, pi_approx);
    
    return 0;
}