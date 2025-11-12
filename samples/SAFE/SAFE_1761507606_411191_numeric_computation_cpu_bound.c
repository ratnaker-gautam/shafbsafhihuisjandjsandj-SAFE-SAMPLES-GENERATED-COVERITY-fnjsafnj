//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000000
#define PI 3.14159265358979323846

double compute_pi_approximation(int iterations) {
    if (iterations <= 0 || iterations > MAX_ITERATIONS) {
        return 0.0;
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

int main() {
    char input_buffer[256];
    int iterations;
    
    printf("Enter number of iterations for pi approximation (1-%d): ", MAX_ITERATIONS);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *endptr;
    long temp_iterations = strtol(input_buffer, &endptr, 10);
    
    if (endptr == input_buffer || *endptr != '\n') {
        fprintf(stderr, "Invalid input: must be an integer\n");
        return 1;
    }
    
    if (temp_iterations < 1 || temp_iterations > MAX_ITERATIONS) {
        fprintf(stderr, "Invalid input: must be between 1 and %d\n", MAX_ITERATIONS);
        return 1;
    }
    
    iterations = (int)temp_iterations;
    
    printf("Computing pi approximation using %d iterations...\n", iterations);
    
    double pi_approx = compute_pi_approximation(iterations);
    double error = fabs(pi_approx - PI);
    
    printf("Approximated pi: %.15f\n", pi_approx);
    printf("Actual pi:       %.15f\n", PI);
    printf("Absolute error:  %.15f\n", error);
    printf("Relative error:  %.15f%%\n", (error / PI) * 100.0);
    
    return 0;
}