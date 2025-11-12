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
    printf("Finding root of cubic equation\n\n");
    
    double guess = safe_input("Enter initial guess: ");
    
    double root = newton_raphson(guess);
    
    if (!isnan(root)) {
        printf("Approximate root: %.10f\n", root);
        double verification = root * root * root - 2 * root - 5;
        printf("f(root) = %.10f\n", verification);
    } else {
        printf("Failed to find root with given initial guess.\n");
    }
    
    return EXIT_SUCCESS;
}