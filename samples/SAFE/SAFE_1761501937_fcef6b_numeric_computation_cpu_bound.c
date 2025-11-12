//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000000

int main() {
    int n;
    printf("Enter number of iterations (1-%d): ", MAX_ITERATIONS);
    
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (n < 1 || n > MAX_ITERATIONS) {
        fprintf(stderr, "Error: Number out of range\n");
        return 1;
    }
    
    double pi_approximation = 0.0;
    int sign = 1;
    
    for (int i = 0; i < n; i++) {
        double term = 1.0 / (2 * i + 1);
        if (sign > 0) {
            pi_approximation += term;
        } else {
            pi_approximation -= term;
        }
        sign = -sign;
    }
    
    pi_approximation *= 4.0;
    
    printf("Approximation of π after %d iterations: %.15f\n", n, pi_approximation);
    
    double error = 3.141592653589793 - pi_approximation;
    if (error < 0) error = -error;
    printf("Absolute error: %.15f\n", error);
    
    return 0;
}