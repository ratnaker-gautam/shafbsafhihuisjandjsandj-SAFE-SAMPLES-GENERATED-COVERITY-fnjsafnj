//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000000

long double compute_pi_approximation(int iterations) {
    if (iterations <= 0 || iterations > MAX_ITERATIONS) {
        return -1.0L;
    }
    
    long double sum = 0.0L;
    for (int i = 0; i < iterations; i++) {
        long double term = 1.0L / (2 * (long double)i + 1);
        if (i % 2 == 0) {
            sum += term;
        } else {
            sum -= term;
        }
    }
    
    return 4.0L * sum;
}

int main(void) {
    char input_buffer[64];
    int iterations;
    
    printf("Enter number of iterations for pi approximation (1-%d): ", MAX_ITERATIONS);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    char *endptr;
    long temp_iterations = strtol(input_buffer, &endptr, 10);
    
    if (endptr == input_buffer || *endptr != '\n' && *endptr != '\0') {
        fprintf(stderr, "Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    if (temp_iterations < 1 || temp_iterations > MAX_ITERATIONS) {
        fprintf(stderr, "Iterations must be between 1 and %d\n", MAX_ITERATIONS);
        return EXIT_FAILURE;
    }
    
    iterations = (int)temp_iterations;
    
    long double result = compute_pi_approximation(iterations);
    if (result < 0.0L) {
        fprintf(stderr, "Computation error\n");
        return EXIT_FAILURE;
    }
    
    printf("Pi approximation after %d iterations: %.15Lf\n", iterations, result);
    
    return EXIT_SUCCESS;
}