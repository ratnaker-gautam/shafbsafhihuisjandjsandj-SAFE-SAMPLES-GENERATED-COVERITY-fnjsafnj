//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: numeric_computation
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
    int choice;
    printf("Numeric Computation Benchmark\n");
    printf("1. Compute Pi approximation\n");
    printf("2. Compute Euler's number approximation\n");
    printf("Enter choice (1 or 2): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (choice != 1 && choice != 2) {
        printf("Invalid choice\n");
        return 1;
    }
    
    int iterations;
    printf("Enter number of iterations (1-%d): ", MAX_ITERATIONS);
    if (scanf("%d", &iterations) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (iterations <= 0 || iterations > MAX_ITERATIONS) {
        printf("Invalid iteration count\n");
        return 1;
    }
    
    double result;
    if (choice == 1) {
        result = compute_pi_approximation(iterations);
        if (result < 0) {
            printf("Computation error\n");
            return 1;
        }
        printf("Pi approximation after %d iterations: %.15f\n", iterations, result);
        printf("Difference from math.h PI: %.15f\n", fabs(result - 3.141592653589793));
    } else {
        result = compute_euler_approximation(iterations);
        if (result < 0) {
            printf("Computation error\n");
            return 1;
        }
        printf("Euler's number approximation after %d iterations: %.15f\n", iterations, result);
        printf("Difference from math.h E: %.15f\n", fabs(result - 2.718281828459045));
    }
    
    return 0;
}