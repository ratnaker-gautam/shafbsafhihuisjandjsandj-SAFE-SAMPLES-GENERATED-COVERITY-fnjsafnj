//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: numeric_computation
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
    double prev_guess;
    int iter = 0;
    
    do {
        prev_guess = guess;
        guess = 0.5 * (guess + x / guess);
        iter++;
    } while (fabs(guess - prev_guess) > TOLERANCE && iter < MAX_ITERATIONS);
    
    return guess;
}

double compute_log_approx(double x) {
    if (x <= 0.0) return NAN;
    
    double result = 0.0;
    double term = (x - 1.0) / (x + 1.0);
    double term_sq = term * term;
    double current_term = term;
    int n = 1;
    
    while (fabs(current_term) > TOLERANCE && n < MAX_ITERATIONS) {
        result += current_term;
        current_term = current_term * term_sq * (2.0 * n - 1.0) / (2.0 * n + 1.0);
        n++;
    }
    
    return 2.0 * result;
}

int main(void) {
    double input_value;
    char buffer[256];
    
    printf("Enter a positive number: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    char *endptr;
    input_value = strtod(buffer, &endptr);
    if (endptr == buffer || *endptr != '\n') {
        fprintf(stderr, "Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    if (input_value <= 0.0 || input_value > MAX_INPUT || input_value < MIN_INPUT) {
        fprintf(stderr, "Input out of valid range (0, %g]\n", MAX_INPUT);
        return EXIT_FAILURE;
    }
    
    if (!isfinite(input_value)) {
        fprintf(stderr, "Input is not finite\n");
        return EXIT_FAILURE;
    }
    
    double sqrt_result = compute_sqrt_newton(input_value);
    double log_result = compute_log_approx(input_value);
    
    if (isnan(sqrt_result) || isnan(log_result)) {
        fprintf(stderr, "Computation error\n");
        return EXIT_FAILURE;
    }
    
    printf("Input: %g\n", input_value);
    printf("Square root (Newton): %.10g\n", sqrt_result);
    printf("Natural log (approx): %.10g\n", log_result);
    printf("Verification - sqrt^2: %.10g\n", sqrt_result * sqrt_result);
    printf("Verification - exp(log): %.10g\n", exp(log_result));
    
    return EXIT_SUCCESS;
}