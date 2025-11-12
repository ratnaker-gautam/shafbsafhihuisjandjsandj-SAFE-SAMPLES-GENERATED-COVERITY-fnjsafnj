//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <limits.h>

#define MAX_ITERATIONS 1000000
#define PI_CONST 3.14159265358979323846

double compute_pi_approximation(int iterations) {
    double sum = 0.0;
    for (int i = 0; i < iterations; i++) {
        if (i == INT_MAX) {
            break;
        }
        double denominator = 2.0 * i + 1.0;
        if (denominator == 0.0) {
            continue;
        }
        double term = 1.0 / denominator;
        if (i % 2 == 0) {
            sum += term;
        } else {
            sum -= term;
        }
    }
    return 4.0 * sum;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <iterations>\n", argv[0]);
        return 1;
    }

    char *endptr;
    long input_iterations = strtol(argv[1], &endptr, 10);
    
    if (endptr == argv[1] || *endptr != '\0') {
        fprintf(stderr, "Error: Invalid number format\n");
        return 1;
    }

    if (input_iterations <= 0) {
        fprintf(stderr, "Error: Iterations must be positive\n");
        return 1;
    }

    if (input_iterations > MAX_ITERATIONS) {
        fprintf(stderr, "Error: Iterations exceed maximum of %d\n", MAX_ITERATIONS);
        return 1;
    }

    if (input_iterations > INT_MAX) {
        fprintf(stderr, "Error: Iterations value too large\n");
        return 1;
    }

    int iterations = (int)input_iterations;
    if (iterations < 0) {
        fprintf(stderr, "Error: Iterations value invalid\n");
        return 1;
    }

    double pi_approx = compute_pi_approximation(iterations);
    if (!isfinite(pi_approx)) {
        fprintf(stderr, "Error: Numerical computation failed\n");
        return 1;
    }

    double error = fabs(pi_approx - PI_CONST);

    printf("Pi approximation after %d iterations: %.15f\n", iterations, pi_approx);
    printf("Actual value of pi: %.15f\n", PI_CONST);
    printf("Absolute error: %.15f\n", error);

    return 0;
}