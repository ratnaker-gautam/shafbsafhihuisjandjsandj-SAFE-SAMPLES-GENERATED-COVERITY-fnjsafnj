//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdint.h>
#include <limits.h>
#include <float.h>

#define MAX_ITERATIONS 1000000
#define MAX_INPUT 1000000

double compute_pi_approximation(int iterations) {
    if (iterations <= 0) {
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

double compute_e_approximation(int iterations) {
    if (iterations <= 0) {
        return 1.0;
    }
    double e = 1.0;
    double factorial = 1.0;
    for (int i = 1; i <= iterations; i++) {
        if (factorial > DBL_MAX / i) {
            break;
        }
        factorial *= i;
        e += 1.0 / factorial;
    }
    return e;
}

int validate_input(const char* input_str, int* value) {
    if (input_str == NULL) {
        return 0;
    }
    
    char* endptr;
    long val = strtol(input_str, &endptr, 10);
    
    if (endptr == input_str || *endptr != '\0') {
        return 0;
    }
    
    if (val < 1 || val > MAX_INPUT) {
        return 0;
    }
    
    *value = (int)val;
    return 1;
}

int main() {
    char input_buffer[256];
    int iterations;
    
    printf("Enter number of iterations (1-%d): ", MAX_INPUT);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (!validate_input(input_buffer, &iterations)) {
        fprintf(stderr, "Invalid input. Please enter a number between 1 and %d\n", MAX_INPUT);
        return 1;
    }
    
    if (iterations > MAX_ITERATIONS) {
        iterations = MAX_ITERATIONS;
        printf("Limiting iterations to %d for performance\n", MAX_ITERATIONS);
    }
    
    clock_t start_time = clock();
    
    double pi_approx = compute_pi_approximation(iterations);
    double e_approx = compute_e_approximation(iterations);
    
    clock_t end_time = clock();
    double cpu_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    
    printf("Results after %d iterations:\n", iterations);
    printf("Pi approximation: %.15f\n", pi_approx);
    printf("E approximation:  %.15f\n", e_approx);
    printf("Computation time: %.6f seconds\n", cpu_time);
    
    return 0;
}