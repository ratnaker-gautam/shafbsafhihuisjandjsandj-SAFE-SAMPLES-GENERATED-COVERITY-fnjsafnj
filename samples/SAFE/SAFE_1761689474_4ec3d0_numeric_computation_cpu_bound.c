//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000000

double compute_series_sum(int n_terms) {
    if (n_terms <= 0 || n_terms > MAX_ITERATIONS) {
        return 0.0;
    }
    
    double sum = 0.0;
    for (int i = 1; i <= n_terms; i++) {
        if (i == 0) continue;
        double denominator = (double)i * (double)i;
        if (denominator == 0.0) continue;
        double term = 1.0 / denominator;
        if (isinf(term) || isnan(term)) {
            break;
        }
        sum += term;
    }
    return sum;
}

double compute_pi_approximation(int iterations) {
    if (iterations <= 0 || iterations > MAX_ITERATIONS) {
        return 0.0;
    }
    
    double sum = 0.0;
    int sign = 1;
    for (int i = 0; i < iterations; i++) {
        double denominator = 2.0 * i + 1.0;
        if (denominator == 0.0) {
            continue;
        }
        double term = (double)sign / denominator;
        if (isinf(term) || isnan(term)) {
            break;
        }
        sum += term;
        sign = -sign;
    }
    return 4.0 * sum;
}

int main() {
    int choice;
    printf("Select computation:\n");
    printf("1. Sum of 1/n^2 series\n");
    printf("2. Pi approximation using Leibniz formula\n");
    
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
        printf("Invalid number of iterations\n");
        return 1;
    }
    
    double result;
    const double pi = 3.14159265358979323846;
    if (choice == 1) {
        result = compute_series_sum(iterations);
        printf("Sum of 1/n^2 series after %d iterations: %.15f\n", iterations, result);
        printf("Expected limit (pi^2/6): %.15f\n", pi * pi / 6.0);
    } else {
        result = compute_pi_approximation(iterations);
        printf("Pi approximation after %d iterations: %.15f\n", iterations, result);
        printf("Actual pi value: %.15f\n", pi);
        printf("Error: %.15f\n", fabs(result - pi));
    }
    
    return 0;
}