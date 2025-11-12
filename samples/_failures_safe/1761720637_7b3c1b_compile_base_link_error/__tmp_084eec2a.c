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
#define ABS(x) ((x) < 0 ? -(x) : (x))

double compute_root(double a, double b, double c) {
    double discriminant = SQUARE(b) - 4.0 * a * c;
    if (discriminant < 0.0) return NAN;
    return (-b + sqrt(discriminant)) / (2.0 * a);
}

double iterative_solver(double target, double initial_guess) {
    double x = initial_guess;
    for (int i = 0; i < MAX_ITERATIONS; i++) {
        double fx = SQUARE(x) - target;
        if (ABS(fx) < EPSILON) return x;
        double derivative = 2.0 * x;
        if (ABS(derivative) < EPSILON) return NAN;
        double delta = fx / derivative;
        if (!VALIDATE_INPUT(x - delta)) return NAN;
        x -= delta;
    }
    return NAN;
}

int main(void) {
    double coefficients[3];
    printf("Enter coefficients a, b, c for quadratic equation ax^2 + bx + c = 0:\n");
    for (int i = 0; i < 3; i++) {
        if (scanf("%lf", &coefficients[i]) != 1) {
            printf("Invalid input.\n");
            return EXIT_FAILURE;
        }
        if (!VALIDATE_INPUT(coefficients[i])) {
            printf("Input out of valid range.\n");
            return EXIT_FAILURE;
        }
    }
    
    if (ABS(coefficients[0]) < EPSILON) {
        printf("Coefficient a cannot be zero.\n");
        return EXIT_FAILURE;
    }
    
    double root = compute_root(coefficients[0], coefficients[1], coefficients[2]);
    if (isnan(root)) {
        printf("No real root found.\n");
        return EXIT_FAILURE;
    }
    
    printf("One root is: %.6f\n", root);
    printf("Verifying by computing square root of constant term:\n");
    
    double constant_term = ABS(coefficients[2]);
    if (!VALIDATE_INPUT(constant_term)) {
        printf("Constant term too large for square root computation.\n");
        return EXIT_FAILURE;
    }
    
    double sqrt_approx = iterative_solver(constant_term, constant_term / 2.0);
    if (isnan(sqrt_approx)) {
        printf("Square root computation failed.\n");
        return EXIT_FAILURE;
    }
    
    printf("Square root of %.6f is approximately: %.6f\n", constant_term, sqrt_approx);
    printf("Difference from library sqrt: %.6f\n", ABS(sqrt_approx - sqrt(constant_term)));
    
    return EXIT_SUCCESS;
}