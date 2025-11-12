//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000000

long double compute_pi_approximation(uint64_t iterations) {
    long double sum = 0.0L;
    for (uint64_t i = 0; i < iterations; i++) {
        long double term = 1.0L / (2.0L * i + 1.0L);
        if (i % 2 == 0) {
            sum += term;
        } else {
            sum -= term;
        }
    }
    return 4.0L * sum;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <iterations>\n", argv[0]);
        return 1;
    }

    char *endptr;
    long input_val = strtol(argv[1], &endptr, 10);
    
    if (endptr == argv[1] || *endptr != '\0') {
        fprintf(stderr, "Error: Invalid number format\n");
        return 1;
    }
    
    if (input_val <= 0) {
        fprintf(stderr, "Error: Iterations must be positive\n");
        return 1;
    }
    
    if (input_val > MAX_ITERATIONS) {
        fprintf(stderr, "Error: Iterations cannot exceed %d\n", MAX_ITERATIONS);
        return 1;
    }
    
    uint64_t iterations = (uint64_t)input_val;
    
    long double pi_approx = compute_pi_approximation(iterations);
    
    printf("Pi approximation after %lu iterations: %.15Lf\n", iterations, pi_approx);
    
    return 0;
}