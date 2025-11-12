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

double newton_raphson(double initial_guess, double (*f)(double), double (*f_prime)(double)) {
    double x = initial_guess;
    for (int i = 0; i < MAX_ITERATIONS; i++) {
        double fx = f(x);
        double fpx = f_prime(x);
        if (fabs(fpx) < EPSILON) {
            break;
        }
        double delta = fx / fpx;
        if (!VALIDATE_INPUT(x - delta)) {
            break;
        }
        x = x - delta;
        if (fabs(delta) < EPSILON) {
            break;
        }
    }
    return x;
}

double polynomial(double x) {
    return CUBE(x) - 2.0 * SQUARE(x) - 5.0 * x + 6.0;
}

double polynomial_derivative(double x) {
    return 3.0 * SQUARE(x) - 4.0 * x - 5.0;
}

int main() {
    double guess;
    printf("Enter initial guess for root finding (between %.1f and %.1f): ", MIN_INPUT, MAX_INPUT);
    if (scanf("%lf", &guess) != 1) {
        fprintf(stderr, "Error: Invalid input format.\n");
        return EXIT_FAILURE;
    }
    
    if (!VALIDATE_INPUT(guess)) {
        fprintf(stderr, "Error: Input out of valid range.\n");
        return EXIT_FAILURE;
    }
    
    double root = newton_raphson(guess, polynomial, polynomial_derivative);
    
    printf("Approximate root: %.10f\n", root);
    printf("Function value at root: %.10f\n", polynomial(root));
    
    return EXIT_SUCCESS;
}