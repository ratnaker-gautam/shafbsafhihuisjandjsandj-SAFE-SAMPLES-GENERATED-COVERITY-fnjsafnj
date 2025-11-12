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

int main() {
    char input_buffer[256];
    int iterations;
    const double PI = 3.14159265358979323846;
    
    printf("Enter number of iterations (1-%d): ", MAX_ITERATIONS);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (sscanf(input_buffer, "%d", &iterations) != 1) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    if (iterations < 1 || iterations > MAX_ITERATIONS) {
        fprintf(stderr, "Iterations must be between 1 and %d\n", MAX_ITERATIONS);
        return 1;
    }
    
    printf("Computing pi approximation using %d iterations...\n", iterations);
    
    double result = compute_pi_approximation(iterations);
    double error = fabs(result - PI);
    
    printf("Approximated pi: %.15f\n", result);
    printf("Actual pi:       %.15f\n", PI);
    printf("Absolute error:  %.15f\n", error);
    
    return 0;
}