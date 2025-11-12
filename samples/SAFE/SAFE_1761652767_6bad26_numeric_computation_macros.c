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
        double dfx = 3.0 * SQUARE(x);
        if (fabs(dfx) < EPSILON) {
            break;
        }
        double delta = fx / dfx;
        if (fabs(delta) < EPSILON) {
            break;
        }
        x = x - delta;
        if (!VALIDATE_INPUT(x)) {
            break;
        }
    }
    return x;
}

int main() {
    double number;
    printf("Enter a number to compute cube root: ");
    if (scanf("%lf", &number) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return EXIT_FAILURE;
    }
    
    if (!VALIDATE_INPUT(number)) {
        fprintf(stderr, "Error: Input out of range [%g, %g]\n", MIN_INPUT, MAX_INPUT);
        return EXIT_FAILURE;
    }
    
    double initial_guess;
    if (number >= 0) {
        initial_guess = 1.0;
    } else {
        initial_guess = -1.0;
    }
    
    double result = newton_raphson(initial_guess, number);
    
    printf("Cube root of %g is approximately %g\n", number, result);
    printf("Verification: %g^3 = %g\n", result, CUBE(result));
    
    return EXIT_SUCCESS;
}