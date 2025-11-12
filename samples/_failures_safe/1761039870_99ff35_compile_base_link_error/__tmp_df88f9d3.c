//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000
#define TOLERANCE 1e-10
#define MAX_INPUT 1000000.0
#define MIN_INPUT 1e-10

double compute_sqrt_newton(double x) {
    if (x < 0.0) return NAN;
    if (x == 0.0) return 0.0;
    
    double guess = x / 2.0;
    for (int i = 0; i < MAX_ITERATIONS; i++) {
        double new_guess = 0.5 * (guess + x / guess);
        if (fabs(new_guess - guess) < TOLERANCE) {
            return new_guess;
        }
        guess = new_guess;
    }
    return guess;
}

double compute_log_approx(double x) {
    if (x <= 0.0) return NAN;
    
    double term = (x - 1.0) / (x + 1.0);
    double term_sq = term * term;
    double sum = term;
    double current_term = term;
    
    for (int i = 1; i < 50; i++) {
        current_term *= term_sq;
        double new_sum = sum + current_term / (2 * i + 1);
        if (new_sum == sum) break;
        sum = new_sum;
    }
    return 2.0 * sum;
}

int main(void) {
    double input;
    char buffer[256];
    
    printf("Enter a positive number: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    char *endptr;
    input = strtod(buffer, &endptr);
    if (endptr == buffer || *endptr != '\n') {
        fprintf(stderr, "Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    if (input <= 0.0) {
        fprintf(stderr, "Input must be positive\n");
        return EXIT_FAILURE;
    }
    
    if (input > MAX_INPUT || input < MIN_INPUT) {
        fprintf(stderr, "Input out of valid range\n");
        return EXIT_FAILURE;
    }
    
    double sqrt_result = compute_sqrt_newton(input);
    double log_result = compute_log_approx(input);
    
    if (isnan(sqrt_result) || isnan(log_result)) {
        fprintf(stderr, "Computation error\n");
        return EXIT_FAILURE;
    }
    
    printf("Number: %.6f\n", input);
    printf("Square root: %.6f\n", sqrt_result);
    printf("Natural logarithm: %.6f\n", log_result);
    printf("Verification - exp(log(x)): %.6f\n", exp(log_result));
    
    return EXIT_SUCCESS;
}