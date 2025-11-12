//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000000

double compute_pi_approximation(int iterations) {
    if (iterations <= 0 || iterations > MAX_ITERATIONS) {
        return 0.0;
    }
    
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

int main(void) {
    printf("Enter number of iterations for pi approximation (1-%d): ", MAX_ITERATIONS);
    
    char input_buffer[32];
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *endptr;
    long input_value = strtol(input_buffer, &endptr, 10);
    
    if (endptr == input_buffer) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    if (*endptr != '\0' && *endptr != '\n') {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    if (input_value <= 0 || input_value > MAX_ITERATIONS) {
        fprintf(stderr, "Input out of valid range\n");
        return 1;
    }
    
    if (input_value > INT_MAX) {
        fprintf(stderr, "Input value too large\n");
        return 1;
    }
    
    int iterations = (int)input_value;
    
    double result = compute_pi_approximation(iterations);
    double actual_pi = 4.0 * atan(1.0);
    double error = fabs(result - actual_pi);
    
    printf("Approximation: %.15f\n", result);
    printf("Actual PI:     %.15f\n", actual_pi);
    printf("Error:         %.15f\n", error);
    
    return 0;
}