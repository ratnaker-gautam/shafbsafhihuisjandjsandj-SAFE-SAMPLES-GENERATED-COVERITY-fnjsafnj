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

double safe_sqrt(double x) {
    if (x < 0.0) {
        return NAN;
    }
    return sqrt(x);
}

double safe_log(double x) {
    if (x <= 0.0) {
        return NAN;
    }
    return log(x);
}

double compute_function(double x) {
    if (isnan(x) || isinf(x)) {
        return NAN;
    }
    double term1 = safe_sqrt(x);
    double term2 = safe_log(x + 1.0);
    if (isnan(term1) || isnan(term2)) {
        return NAN;
    }
    return term1 + term2;
}

double compute_derivative(double x) {
    if (isnan(x) || isinf(x) || x <= -1.0) {
        return NAN;
    }
    double term1 = 1.0 / (2.0 * safe_sqrt(x));
    double term2 = 1.0 / (x + 1.0);
    if (isnan(term1) || isnan(term2)) {
        return NAN;
    }
    return term1 + term2;
}

int main(void) {
    double initial_guess;
    char input_buffer[256];
    
    printf("Enter initial guess for root finding (1e-10 to 1000000): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    char *endptr;
    initial_guess = strtod(input_buffer, &endptr);
    if (endptr == input_buffer) {
        fprintf(stderr, "Invalid input: not a number\n");
        return EXIT_FAILURE;
    }
    
    if (initial_guess < MIN_INPUT || initial_guess > MAX_INPUT) {
        fprintf(stderr, "Input out of valid range\n");
        return EXIT_FAILURE;
    }
    
    double x = initial_guess;
    int iterations = 0;
    
    while (iterations < MAX_ITERATIONS) {
        double fx = compute_function(x);
        double dfx = compute_derivative(x);
        
        if (isnan(fx) || isnan(dfx) || dfx == 0.0) {
            fprintf(stderr, "Numerical method failed\n");
            return EXIT_FAILURE;
        }
        
        double delta = fx / dfx;
        if (isnan(delta) || isinf(delta)) {
            fprintf(stderr, "Numerical instability detected\n");
            return EXIT_FAILURE;
        }
        
        x = x - delta;
        
        if (fabs(delta) < TOLERANCE) {
            break;
        }
        
        iterations++;
    }
    
    if (iterations >= MAX_ITERATIONS) {
        fprintf(stderr, "Maximum iterations reached\n");
        return EXIT_FAILURE;
    }
    
    double final_value = compute_function(x);
    if (isnan(final_value) || isinf(final_value)) {
        fprintf(stderr, "Invalid result\n");
        return EXIT_FAILURE;
    }
    
    printf("Approximate root: %.10f\n", x);
    printf("Function value at root: %.10f\n", final_value);
    printf("Iterations: %d\n", iterations);
    
    return EXIT_SUCCESS;
}