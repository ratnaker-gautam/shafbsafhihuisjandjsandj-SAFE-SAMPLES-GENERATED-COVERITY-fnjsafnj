//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000000

long double compute_pi_approximation(int iterations) {
    if (iterations <= 0 || iterations > MAX_ITERATIONS) {
        return 0.0L;
    }
    
    long double sum = 0.0L;
    for (int i = 0; i < iterations; i++) {
        long double term = 1.0L / (2 * i + 1);
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
        return 1;
    }
    
    char *endptr;
    long temp_iterations = strtol(input_buffer, &endptr, 10);
    
    if (endptr == input_buffer || *endptr != '\n') {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    if (temp_iterations < 1 || temp_iterations > MAX_ITERATIONS) {
        fprintf(stderr, "Iterations must be between 1 and %d\n", MAX_ITERATIONS);
        return 1;
    }
    
    iterations = (int)temp_iterations;
    
    long double pi_approx = compute_pi_approximation(iterations);
    
    printf("Pi approximation after %d iterations: %.15Lf\n", iterations, pi_approx);
    printf("Difference from actual pi: %.15Lf\n", pi_approx - 3.14159265358979323846L);
    
    return 0;
}