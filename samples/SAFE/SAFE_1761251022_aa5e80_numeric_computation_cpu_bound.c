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
        return -1.0;
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

int main(void) {
    printf("Enter number of iterations for pi approximation (1-%d): ", MAX_ITERATIONS);
    
    char input_buffer[256];
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *endptr;
    long iterations_long = strtol(input_buffer, &endptr, 10);
    
    if (endptr == input_buffer || *endptr != '\n' && *endptr != '\0') {
        fprintf(stderr, "Invalid input: not a valid number\n");
        return 1;
    }
    
    if (iterations_long <= 0 || iterations_long > MAX_ITERATIONS) {
        fprintf(stderr, "Invalid input: must be between 1 and %d\n", MAX_ITERATIONS);
        return 1;
    }
    
    if (iterations_long > INT_MAX) {
        fprintf(stderr, "Input too large\n");
        return 1;
    }
    
    int iterations = (int)iterations_long;
    
    printf("Computing pi approximation with %d iterations...\n", iterations);
    
    double result = compute_pi_approximation(iterations);
    if (result < 0) {
        fprintf(stderr, "Computation error\n");
        return 1;
    }
    
    printf("Approximation: %.15f\n", result);
    printf("Actual pi:     %.15f\n", PI);
    printf("Difference:    %.15f\n", fabs(result - PI));
    
    return 0;
}