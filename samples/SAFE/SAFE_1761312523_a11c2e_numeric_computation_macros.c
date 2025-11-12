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
    int iterations = 0;
    
    while (iterations < MAX_ITERATIONS) {
        double fx = CUBE(x) - target;
        double dfx = 3.0 * SQUARE(x);
        
        if (fabs(dfx) < EPSILON) {
            break;
        }
        
        double delta = fx / dfx;
        x = x - delta;
        
        if (fabs(delta) < EPSILON) {
            break;
        }
        
        iterations++;
    }
    
    return x;
}

int main() {
    double target_value;
    double initial_guess;
    
    printf("Enter target value for cube root calculation: ");
    if (scanf("%lf", &target_value) != 1) {
        fprintf(stderr, "Error: Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    if (!VALIDATE_INPUT(target_value)) {
        fprintf(stderr, "Error: Input value out of range [%g, %g]\n", MIN_INPUT, MAX_INPUT);
        return EXIT_FAILURE;
    }
    
    printf("Enter initial guess: ");
    if (scanf("%lf", &initial_guess) != 1) {
        fprintf(stderr, "Error: Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    if (!VALIDATE_INPUT(initial_guess)) {
        fprintf(stderr, "Error: Initial guess out of range [%g, %g]\n", MIN_INPUT, MAX_INPUT);
        return EXIT_FAILURE;
    }
    
    if (fabs(initial_guess) < EPSILON && fabs(target_value) > EPSILON) {
        fprintf(stderr, "Error: Invalid initial guess for non-zero target\n");
        return EXIT_FAILURE;
    }
    
    double result = newton_raphson(initial_guess, target_value);
    double verification = CUBE(result);
    
    printf("Calculated cube root: %.10g\n", result);
    printf("Verification (cube of result): %.10g\n", verification);
    printf("Target value: %.10g\n", target_value);
    printf("Absolute error: %.10g\n", fabs(verification - target_value));
    
    return EXIT_SUCCESS;
}