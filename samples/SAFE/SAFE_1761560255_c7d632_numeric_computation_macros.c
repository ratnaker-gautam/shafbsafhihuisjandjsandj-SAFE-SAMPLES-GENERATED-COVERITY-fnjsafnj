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

#define VALIDATE_INPUT(x) ((x) >= MIN_INPUT && (x) <= MAX_INPUT)
#define SQUARE(x) ((x) * (x))
#define CUBE(x) ((x) * (x) * (x))

double newton_raphson(double initial_guess, double target) {
    double x = initial_guess;
    for (int i = 0; i < MAX_ITERATIONS; i++) {
        double fx = CUBE(x) - target;
        if (fabs(fx) < EPSILON) {
            return x;
        }
        double fpx = 3.0 * SQUARE(x);
        if (fabs(fpx) < EPSILON) {
            return NAN;
        }
        double delta = fx / fpx;
        if (fabs(delta) < EPSILON) {
            return x;
        }
        x = x - delta;
        if (!VALIDATE_INPUT(x)) {
            return NAN;
        }
    }
    return NAN;
}

int main() {
    double target, initial_guess;
    
    printf("Enter target value for cube root calculation: ");
    if (scanf("%lf", &target) != 1) {
        fprintf(stderr, "Error: Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    if (!VALIDATE_INPUT(target)) {
        fprintf(stderr, "Error: Input out of valid range [%g, %g]\n", MIN_INPUT, MAX_INPUT);
        return EXIT_FAILURE;
    }
    
    printf("Enter initial guess: ");
    if (scanf("%lf", &initial_guess) != 1) {
        fprintf(stderr, "Error: Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    if (!VALIDATE_INPUT(initial_guess)) {
        fprintf(stderr, "Error: Initial guess out of valid range [%g, %g]\n", MIN_INPUT, MAX_INPUT);
        return EXIT_FAILURE;
    }
    
    double result = newton_raphson(initial_guess, target);
    
    if (isnan(result)) {
        printf("Error: Failed to converge to a solution\n");
        return EXIT_FAILURE;
    }
    
    printf("Cube root of %g is approximately: %.10g\n", target, result);
    printf("Verification: %.10g^3 = %.10g\n", result, CUBE(result));
    
    return EXIT_SUCCESS;
}