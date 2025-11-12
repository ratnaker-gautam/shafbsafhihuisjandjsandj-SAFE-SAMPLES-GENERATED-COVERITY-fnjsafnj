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
        long double term = 1.0L / (2 * i + 1);
        pi_approx += sign * term;
        sign = -sign;
        
        if (i >= n && i % 1000 == 0) {
            long double current_pi = pi_approx * 4.0L;
            printf("Iteration %d: π ≈ %.15Lf\n", i, current_pi);
        }
    }
    
    long double final_pi = pi_approx * 4.0L;
    printf("Final approximation after %d iterations: π ≈ %.15Lf\n", MAX_ITERATIONS, final_pi);
    
    long double error = 3.14159265358979323846L - final_pi;
    if (error < 0) error = -error;
    printf("Absolute error: %.15Lf\n", error);
    
    return 0;
}