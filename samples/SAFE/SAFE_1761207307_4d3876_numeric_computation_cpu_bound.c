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

double compute_e_approximation(int iterations) {
    if (iterations <= 0 || iterations > MAX_ITERATIONS) {
        return -1.0;
    }
    
    double e = 1.0;
    double factorial = 1.0;
    
    for (int i = 1; i <= iterations; i++) {
        if (factorial > DBL_MAX / i) {
            return -1.0;
        }
        factorial *= i;
        double term = 1.0 / factorial;
        if (e > DBL_MAX - term) {
            return -1.0;
        }
        e += term;
    }
    return e;
}

int main() {
    int choice;
    printf("Select computation:\n");
    printf("1. Approximate Pi using Leibniz formula\n");
    printf("2. Approximate e using Taylor series\n");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    int iterations;
    printf("Enter number of iterations (1-%d): ", MAX_ITERATIONS);
    if (scanf("%d", &iterations) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (iterations <= 0 || iterations > MAX_ITERATIONS) {
        printf("Invalid number of iterations\n");
        return 1;
    }
    
    double result;
    switch (choice) {
        case 1:
            result = compute_pi_approximation(iterations);
            if (result < 0) {
                printf("Computation failed\n");
                return 1;
            }
            printf("Pi approximation after %d iterations: %.15f\n", iterations, result);
            printf("Difference from math.h pi: %.15f\n", fabs(result - 3.14159265358979323846));
            break;
        case 2:
            result = compute_e_approximation(iterations);
            if (result < 0) {
                printf("Computation failed\n");
                return 1;
            }
            printf("e approximation after %d iterations: %.15f\n", iterations, result);
            printf("Difference from math.h e: %.15f\n", fabs(result - 2.71828182845904523536));
            break;
        default:
            printf("Invalid choice\n");
            return 1;
    }
    
    return 0;
}