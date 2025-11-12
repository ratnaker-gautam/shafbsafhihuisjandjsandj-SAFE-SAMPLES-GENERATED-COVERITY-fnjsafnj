//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10
#define MAX_INPUT 1000000.0
#define MIN_INPUT -1000000.0
#define IS_VALID(x) ((x) >= MIN_INPUT && (x) <= MAX_INPUT)

double newton_raphson(double initial_guess, double (*f)(double), double (*f_prime)(double)) {
    double x = initial_guess;
    for (int i = 0; i < MAX_ITERATIONS; i++) {
        double fx = f(x);
        double fpx = f_prime(x);
        if (fabs(fpx) < EPSILON) {
            break;
        }
        double delta = fx / fpx;
        if (!IS_VALID(x - delta)) {
            break;
        }
        x = x - delta;
        if (fabs(fx) < EPSILON) {
            break;
        }
    }
    return x;
}

double sample_function(double x) {
    return x * x - 2.0;
}

double sample_derivative(double x) {
    return 2.0 * x;
}

int main(void) {
    double initial_guess;
    printf("Enter initial guess for square root of 2: ");
    if (scanf("%lf", &initial_guess) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return EXIT_FAILURE;
    }
    
    if (!IS_VALID(initial_guess)) {
        fprintf(stderr, "Error: Input out of valid range [%g, %g]\n", MIN_INPUT, MAX_INPUT);
        return EXIT_FAILURE;
    }
    
    if (fabs(initial_guess) < EPSILON) {
        fprintf(stderr, "Error: Initial guess too close to zero\n");
        return EXIT_FAILURE;
    }
    
    double result = newton_raphson(initial_guess, sample_function, sample_derivative);
    
    printf("Approximated root: %.10f\n", result);
    printf("Verification: f(root) = %.10f\n", sample_function(result));
    printf("Expected: sqrt(2) ≈ 1.4142135624\n");
    
    return EXIT_SUCCESS;
}