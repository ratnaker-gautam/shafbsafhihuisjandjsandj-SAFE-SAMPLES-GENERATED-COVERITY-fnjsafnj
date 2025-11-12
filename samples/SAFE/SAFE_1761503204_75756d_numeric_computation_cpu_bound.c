//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define MAX_ITERATIONS 1000000

double compute_pi_approximation(int iterations) {
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
        fprintf(stderr, "Error: Iterations exceed maximum allowed (%d)\n", MAX_ITERATIONS);
        return 1;
    }

    int iterations = (int)input_iterations;
    if (iterations != input_iterations) {
        fprintf(stderr, "Error: Iterations value too large\n");
        return 1;
    }

    double pi_approx = compute_pi_approximation(iterations);
    double actual_pi = 3.14159265358979323846;
    double error = fabs(pi_approx - actual_pi);

    printf("Pi approximation after %d iterations: %.15f\n", iterations, pi_approx);
    printf("Actual value of pi: %.15f\n", actual_pi);
    printf("Absolute error: %.15f\n", error);

    return 0;
}