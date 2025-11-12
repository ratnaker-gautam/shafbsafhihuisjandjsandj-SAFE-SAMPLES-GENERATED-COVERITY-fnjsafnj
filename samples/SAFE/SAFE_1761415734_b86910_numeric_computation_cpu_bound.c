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
    
    long double pi_approx = 0.0L;
    int iterations = n * 1000;
    
    if (iterations > MAX_ITERATIONS) {
        iterations = MAX_ITERATIONS;
    }
    
    for (int i = 0; i < iterations; i++) {
        long double term = 1.0L / (2 * i + 1);
        if (i % 2 == 0) {
            pi_approx += term;
        } else {
            pi_approx -= term;
        }
    }
    
    pi_approx *= 4.0L;
    
    long double area_sum = 0.0L;
    for (int i = 0; i < iterations; i++) {
        long double x = (long double)i / iterations;
        long double y = 4.0L / (1.0L + x * x);
        area_sum += y;
    }
    
    long double pi_approx2 = area_sum / iterations;
    
    long double final_pi = (pi_approx + pi_approx2) / 2.0L;
    
    printf("Computed pi approximation: %.15Lf\n", final_pi);
    printf("Iterations performed: %d\n", iterations);
    
    long double error = final_pi - 3.14159265358979323846L;
    if (error < 0) error = -error;
    printf("Absolute error: %.15Lf\n", error);
    
    return 0;
}