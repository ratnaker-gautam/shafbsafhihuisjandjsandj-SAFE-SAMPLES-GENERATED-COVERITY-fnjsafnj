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
            return x;
        }
        double delta = fx / fpx;
        if (!VALIDATE_INPUT(x - delta)) {
            return x;
        }
        x = x - delta;
    }
    return x;
}

int main() {
    double target_value;
    printf("Enter a number to compute cube root: ");
    if (scanf("%lf", &target_value) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return EXIT_FAILURE;
    }
    
    if (!VALIDATE_INPUT(target_value)) {
        fprintf(stderr, "Error: Input out of valid range [%g, %g]\n", MIN_INPUT, MAX_INPUT);
        return EXIT_FAILURE;
    }
    
    double initial_guess;
    if (target_value >= 0.0) {
        initial_guess = 1.0;
    } else {
        initial_guess = -1.0;
    }
    
    double result = newton_raphson(initial_guess, target_value);
    
    printf("Cube root of %g is approximately %g\n", target_value, result);
    printf("Verification: %g^3 = %g\n", result, CUBE(result));
    
    return EXIT_SUCCESS;
}