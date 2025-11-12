//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define TOLERANCE 1e-10

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
    
    double result = 1.0;
    double factorial = 1.0;
    
    for (int i = 1; i <= iterations; i++) {
        if (factorial > 1e308 / i) {
            return -1.0;
        }
        factorial *= i;
        result += 1.0 / factorial;
    }
    return result;
}

int main(void) {
    int choice;
    int iterations;
    
    printf("Numeric Computation Demo\n");
    printf("1. Approximate Pi using Leibniz formula\n");
    printf("2. Approximate e using Taylor series\n");
    printf("Enter choice (1 or 2): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (choice != 1 && choice != 2) {
        printf("Invalid choice\n");
        return 1;
    }
    
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
    if (choice == 1) {
        result = compute_pi_approximation(iterations);
        if (result < 0) {
            printf("Computation error\n");
            return 1;
        }
        printf("Pi approximation after %d iterations: %.15f\n", iterations, result);
        printf("Difference from math.h pi: %.15f\n", fabs(result - 3.14159265358979323846));
    } else {
        result = compute_e_approximation(iterations);
        if (result < 0) {
            printf("Computation error\n");
            return 1;
        }
        printf("e approximation after %d iterations: %.15f\n", iterations, result);
        printf("Difference from math.h e: %.15f\n", fabs(result - 2.71828182845904523536));
    }
    
    return 0;
}