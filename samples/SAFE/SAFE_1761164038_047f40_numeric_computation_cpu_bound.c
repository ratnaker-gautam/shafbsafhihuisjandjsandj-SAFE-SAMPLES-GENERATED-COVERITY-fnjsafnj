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
    printf("Enter number of iterations for pi approximation (1-%d): ", MAX_ITERATIONS);
    
    char input_buffer[32];
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *endptr;
    long input_value = strtol(input_buffer, &endptr, 10);
    if (endptr == input_buffer || *endptr != '\n' && *endptr != '\0') {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    if (input_value <= 0 || input_value > MAX_ITERATIONS) {
        fprintf(stderr, "Input out of valid range\n");
        return 1;
    }
    
    int iterations = (int)input_value;
    if (iterations != input_value) {
        fprintf(stderr, "Input value too large\n");
        return 1;
    }
    
    long double result = compute_pi_approximation(iterations);
    if (result < 0) {
        fprintf(stderr, "Computation error\n");
        return 1;
    }
    
    printf("Pi approximation after %d iterations: %.15Lf\n", iterations, result);
    
    return 0;
}