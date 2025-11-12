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
        if (fabs(delta) < EPSILON) {
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

int main() {
    double guess;
    char buffer[256];
    
    printf("Enter initial guess for Newton-Raphson (find sqrt(2)): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *endptr;
    guess = strtod(buffer, &endptr);
    if (endptr == buffer || *endptr != '\n') {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    if (!IS_VALID(guess)) {
        fprintf(stderr, "Input out of valid range [%g, %g]\n", MIN_INPUT, MAX_INPUT);
        return 1;
    }
    
    if (fabs(guess) < EPSILON) {
        fprintf(stderr, "Initial guess too close to zero\n");
        return 1;
    }
    
    double result = newton_raphson(guess, sample_function, sample_derivative);
    
    printf("Approximated root: %.10f\n", result);
    printf("Verification (should be near zero): %.10f\n", sample_function(result));
    
    return 0;
}