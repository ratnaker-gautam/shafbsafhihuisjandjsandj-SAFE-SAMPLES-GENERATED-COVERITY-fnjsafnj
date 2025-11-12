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

double safe_input(const char* prompt) {
    char buffer[256];
    double value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            fprintf(stderr, "Input error\n");
            exit(EXIT_FAILURE);
        }
        
        char* endptr;
        value = strtod(buffer, &endptr);
        
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter a valid number.\n");
            continue;
        }
        
        if (value > MAX_INPUT || value < MIN_INPUT) {
            printf("Input out of range. Must be between %.1f and %.1f.\n", MIN_INPUT, MAX_INPUT);
            continue;
        }
        
        break;
    }
    
    return value;
}

double newton_raphson(double initial_guess) {
    double x = initial_guess;
    int iterations = 0;
    
    while (iterations < MAX_ITERATIONS) {
        double fx = x * x * x - 2 * x - 5;
        double dfx = 3 * x * x - 2;
        
        if (fabs(dfx) < EPSILON) {
            fprintf(stderr, "Derivative too small\n");
            return NAN;
        }
        
        double delta = fx / dfx;
        
        if (fabs(delta) < EPSILON) {
            break;
        }
        
        x = x - delta;
        
        if (!isfinite(x)) {
            fprintf(stderr, "Numerical instability detected\n");
            return NAN;
        }
        
        iterations++;
    }
    
    if (iterations >= MAX_ITERATIONS) {
        fprintf(stderr, "Maximum iterations reached\n");
        return NAN;
    }
    
    return x;
}

int main(void) {
    printf("Newton-Raphson Method for f(x) = x^3 - 2x - 5\n");
    printf("Finding root between 2 and 3\n\n");
    
    double guess = safe_input("Enter initial guess (between 2 and 3): ");
    
    if (guess < 2.0 || guess > 3.0) {
        fprintf(stderr, "Initial guess must be between 2 and 3\n");
        return EXIT_FAILURE;
    }
    
    double root = newton_raphson(guess);
    
    if (isnan(root)) {
        fprintf(stderr, "Failed to find root\n");
        return EXIT_FAILURE;
    }
    
    printf("Approximate root: %.10f\n", root);
    printf("Verification f(root) = %.10f\n", root * root * root - 2 * root - 5);
    
    return EXIT_SUCCESS;
}