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

double newton_raphson(double initial_guess, double target) {
    double x = initial_guess;
    for (int i = 0; i < MAX_ITERATIONS; i++) {
        double fx = SQUARE(x) - target;
        if (ABS(fx) < EPSILON) {
            return x;
        }
        double dfx = 2.0 * x;
        if (ABS(dfx) < EPSILON) {
            break;
        }
        double delta = fx / dfx;
        if (!VALIDATE_INPUT(x - delta)) {
            break;
        }
        x = x - delta;
    }
    return -1.0;
}

int main() {
    double number, initial_guess;
    
    printf("Enter a number to find square root: ");
    if (scanf("%lf", &number) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (!VALIDATE_INPUT(number) || number < 0) {
        printf("Number must be between 0 and %f\n", MAX_INPUT);
        return 1;
    }
    
    printf("Enter initial guess: ");
    if (scanf("%lf", &initial_guess) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (!VALIDATE_INPUT(initial_guess) || initial_guess <= 0) {
        printf("Initial guess must be positive and within bounds\n");
        return 1;
    }
    
    double result = newton_raphson(initial_guess, number);
    
    if (result < 0) {
        printf("Failed to converge\n");
        return 1;
    }
    
    printf("Approximate square root: %.10f\n", result);
    printf("Verification: %.10f^2 = %.10f\n", result, SQUARE(result));
    
    return 0;
}