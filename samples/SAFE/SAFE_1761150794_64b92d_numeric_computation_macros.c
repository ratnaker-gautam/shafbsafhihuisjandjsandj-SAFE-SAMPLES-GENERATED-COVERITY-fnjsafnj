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
            fprintf(stderr, "Invalid input. Please enter a valid number.\n");
            continue;
        }
        
        if (value > MAX_INPUT || value < MIN_INPUT) {
            fprintf(stderr, "Input out of range. Must be between %g and %g.\n", MIN_INPUT, MAX_INPUT);
            continue;
        }
        
        break;
    }
    
    return value;
}

double newton_raphson(double initial_guess) {
    double x = initial_guess;
    double x_prev;
    int iterations = 0;
    
    do {
        x_prev = x;
        double fx = x_prev * x_prev * x_prev - 2.0 * x_prev - 5.0;
        double fpx = 3.0 * x_prev * x_prev - 2.0;
        
        if (fabs(fpx) < EPSILON) {
            fprintf(stderr, "Derivative too small. Method fails.\n");
            return NAN;
        }
        
        x = x_prev - fx / fpx;
        iterations++;
        
        if (iterations > MAX_ITERATIONS) {
            fprintf(stderr, "Maximum iterations reached.\n");
            return NAN;
        }
        
    } while (fabs(x - x_prev) > EPSILON);
    
    return x;
}

int main(void) {
    printf("Newton-Raphson Method for f(x) = x^3 - 2x - 5\n");
    
    double guess = safe_input("Enter initial guess: ");
    
    double root = newton_raphson(guess);
    
    if (!isnan(root)) {
        printf("Approximate root: %.10f\n", root);
        double verification = root * root * root - 2.0 * root - 5.0;
        printf("f(root) = %.10f\n", verification);
    } else {
        printf("Failed to find root.\n");
    }
    
    return EXIT_SUCCESS;
}