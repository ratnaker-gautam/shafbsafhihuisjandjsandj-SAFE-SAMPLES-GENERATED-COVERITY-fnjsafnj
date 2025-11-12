//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000000

long double compute_pi_approximation(uint64_t iterations) {
    long double sum = 0.0L;
    for (uint64_t i = 0; i < iterations; i++) {
        if (i > UINT64_MAX - 1) break;
        long double term = 1.0L / (2.0L * i + 1.0L);
        if (i % 2 == 0) {
            sum += term;
        } else {
            sum -= term;
        }
    }
    return 4.0L * sum;
}

int main(void) {
    char input_buffer[256];
    uint64_t iterations;
    
    printf("Enter number of iterations for pi approximation (1-%u): ", MAX_ITERATIONS);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    char *endptr;
    long long temp = strtoll(input_buffer, &endptr, 10);
    
    if (endptr == input_buffer || *endptr != '\n') {
        fprintf(stderr, "Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    if (temp <= 0 || temp > MAX_ITERATIONS) {
        fprintf(stderr, "Iterations must be between 1 and %u\n", MAX_ITERATIONS);
        return EXIT_FAILURE;
    }
    
    iterations = (uint64_t)temp;
    
    long double pi_approx = compute_pi_approximation(iterations);
    
    printf("Pi approximation after %lu iterations: %.15Lf\n", iterations, pi_approx);
    
    return EXIT_SUCCESS;
}