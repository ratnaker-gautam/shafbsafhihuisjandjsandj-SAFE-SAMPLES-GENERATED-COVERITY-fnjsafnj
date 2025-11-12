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
    if (x < 0.0) return NAN;
    return sqrt(x);
}

double safe_log(double x) {
    if (x <= 0.0) return NAN;
    return log(x);
}

double compute_function(double x) {
    double term1 = safe_sqrt(x);
    if (isnan(term1)) return NAN;
    
    double term2 = safe_log(x + 1.0);
    if (isnan(term2)) return NAN;
    
    double term3 = sin(x);
    
    return term1 + term2 + term3;
}

double compute_derivative(double x) {
    double h = 1e-8;
    double f1 = compute_function(x + h);
    double f2 = compute_function(x - h);
    
    if (isnan(f1) || isnan(f2)) return NAN;
    
    return (f1 - f2) / (2.0 * h);
}

int find_root_newton(double initial_guess, double *result) {
    double x = initial_guess;
    int iterations = 0;
    
    while (iterations < MAX_ITERATIONS) {
        double fx = compute_function(x);
        double dfx = compute_derivative(x);
        
        if (isnan(fx) || isnan(dfx)) return 0;
        if (fabs(dfx) < TOLERANCE) return 0;
        
        double delta = fx / dfx;
        if (fabs(delta) > MAX_INPUT) return 0;
        
        x = x - delta;
        
        if (fabs(fx) < TOLERANCE) {
            *result = x;
            return 1;
        }
        
        iterations++;
    }
    
    return 0;
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
    
    double root;
    if (find_root_newton(initial_guess, &root)) {
        printf("Found root at: %.10f\n", root);
        printf("Function value at root: %.10f\n", compute_function(root));
    } else {
        printf("Failed to find root with given initial guess\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}