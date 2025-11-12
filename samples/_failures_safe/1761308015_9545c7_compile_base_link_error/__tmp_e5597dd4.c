//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10
#define MAX_INPUT 1000000.0
#define MIN_INPUT 1e-10

double compute_sqrt_newton(double x) {
    if (x < 0) return NAN;
    if (x == 0) return 0;
    
    double guess = x / 2.0;
    double prev_guess;
    int iterations = 0;
    
    do {
        prev_guess = guess;
        guess = 0.5 * (guess + x / guess);
        iterations++;
    } while (fabs(guess - prev_guess) > EPSILON && iterations < MAX_ITERATIONS);
    
    return guess;
}

double compute_log_approx(double x) {
    if (x <= 0) return NAN;
    
    double term = (x - 1) / (x + 1);
    double term_sq = term * term;
    double sum = term;
    double current_term = term;
    int n = 1;
    
    while (fabs(current_term) > EPSILON && n < MAX_ITERATIONS) {
        current_term *= term_sq;
        sum += current_term / (2 * n + 1);
        n++;
    }
    
    return 2 * sum;
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
    
    if (input_value <= 0 || input_value > MAX_INPUT || input_value < MIN_INPUT) {
        fprintf(stderr, "Input out of valid range (0 < x <= %g)\n", MAX_INPUT);
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
    
    printf("Number: %g\n", input_value);
    printf("Square root (Newton): %.10g\n", sqrt_result);
    printf("Square root (library): %.10g\n", sqrt(input_value));
    printf("Natural log (approx): %.10g\n", log_result);
    printf("Natural log (library): %.10g\n", log(input_value));
    
    return EXIT_SUCCESS;
}