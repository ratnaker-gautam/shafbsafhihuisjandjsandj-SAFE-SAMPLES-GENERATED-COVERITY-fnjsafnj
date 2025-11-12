//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>
#include <float.h>

#define MAX_ITERATIONS 1000000

double compute_pi_approximation(int iterations) {
    if (iterations <= 0 || iterations > MAX_ITERATIONS) {
        return -1.0;
    }
    
    double sum = 0.0;
    for (int i = 0; i < iterations; i++) {
        double term = 1.0 / (2.0 * i + 1.0);
        if (i % 2 == 0) {
            sum += term;
        } else {
            sum -= term;
        }
    }
    return 4.0 * sum;
}

double compute_euler_approximation(int iterations) {
    if (iterations <= 0 || iterations > MAX_ITERATIONS) {
        return -1.0;
    }
    
    double result = 1.0;
    double factorial = 1.0;
    
    for (int i = 1; i <= iterations; i++) {
        if (factorial > DBL_MAX / i) {
            return -1.0;
        }
        factorial *= i;
        double term = 1.0 / factorial;
        if (result > DBL_MAX - term) {
            return -1.0;
        }
        result += term;
    }
    return result;
}

int main() {
    printf("Numeric Computation Benchmark\n");
    printf("=============================\n");
    
    int iterations;
    printf("Enter number of iterations (1-%d): ", MAX_ITERATIONS);
    
    if (scanf("%d", &iterations) != 1) {
        printf("Error: Invalid input\n");
        return 1;
    }
    
    if (iterations <= 0 || iterations > MAX_ITERATIONS) {
        printf("Error: Iterations must be between 1 and %d\n", MAX_ITERATIONS);
        return 1;
    }
    
    printf("\nComputing approximations with %d iterations...\n", iterations);
    
    double pi_approx = compute_pi_approximation(iterations);
    if (pi_approx < 0) {
        printf("Error: Failed to compute pi approximation\n");
        return 1;
    }
    
    double euler_approx = compute_euler_approximation(iterations);
    if (euler_approx < 0) {
        printf("Error: Failed to compute Euler's number approximation\n");
        return 1;
    }
    
    printf("\nResults:\n");
    printf("Pi approximation: %.15f\n", pi_approx);
    printf("Euler's number approximation: %.15f\n", euler_approx);
    printf("Difference from actual Pi: %.15f\n", fabs(pi_approx - 3.14159265358979323846));
    printf("Difference from actual e: %.15f\n", fabs(euler_approx - 2.71828182845904523536));
    
    double total = 0.0;
    for (int i = 0; i < iterations; i++) {
        double x = (double)i / iterations;
        total += sin(x) * cos(x) * exp(-x * x);
    }
    
    printf("Additional computation result: %.15f\n", total);
    
    return 0;
}