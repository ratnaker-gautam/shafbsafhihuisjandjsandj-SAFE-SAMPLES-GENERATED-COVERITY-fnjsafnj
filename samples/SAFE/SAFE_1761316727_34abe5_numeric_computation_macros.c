//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10
#define SQUARE(x) ((x) * (x))
#define ABS(x) ((x) < 0 ? -(x) : (x))
#define IS_ZERO(x) (ABS(x) < EPSILON)

double newton_raphson(double initial_guess, int max_iter, double *result) {
    double x = initial_guess;
    double fx, dfx;
    int iter = 0;
    
    while (iter < max_iter) {
        fx = SQUARE(x) - 2.0;
        if (IS_ZERO(fx)) {
            *result = x;
            return 0.0;
        }
        dfx = 2.0 * x;
        if (IS_ZERO(dfx)) {
            return -1.0;
        }
        double delta = fx / dfx;
        if (ABS(delta) < EPSILON) {
            *result = x;
            return 0.0;
        }
        x = x - delta;
        iter++;
    }
    return -1.0;
}

int main() {
    double guess, result;
    char buffer[256];
    
    printf("Enter initial guess for sqrt(2) calculation: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *endptr;
    guess = strtod(buffer, &endptr);
    if (endptr == buffer || *endptr != '\n') {
        fprintf(stderr, "Invalid input: must be a number\n");
        return 1;
    }
    
    if (guess <= 0.0) {
        fprintf(stderr, "Initial guess must be positive\n");
        return 1;
    }
    
    if (guess > 1e6) {
        fprintf(stderr, "Initial guess too large\n");
        return 1;
    }
    
    double error = newton_raphson(guess, MAX_ITERATIONS, &result);
    
    if (error < 0.0) {
        fprintf(stderr, "Failed to converge within %d iterations\n", MAX_ITERATIONS);
        return 1;
    }
    
    printf("Approximated sqrt(2): %.10f\n", result);
    printf("Actual sqrt(2):       %.10f\n", sqrt(2.0));
    printf("Absolute error:       %.10f\n", ABS(result - sqrt(2.0)));
    
    return 0;
}