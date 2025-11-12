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
    
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        exit(EXIT_FAILURE);
    }
    
    char* endptr;
    value = strtod(buffer, &endptr);
    if (endptr == buffer || *endptr != '\n') {
        fprintf(stderr, "Invalid input\n");
        exit(EXIT_FAILURE);
    }
    
    if (value > MAX_INPUT || value < MIN_INPUT) {
        fprintf(stderr, "Input out of range\n");
        exit(EXIT_FAILURE);
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
            fprintf(stderr, "Derivative too small\n");
            exit(EXIT_FAILURE);
        }
        
        x = x_prev - fx / fpx;
        iterations++;
        
        if (iterations > MAX_ITERATIONS) {
            fprintf(stderr, "Maximum iterations exceeded\n");
            exit(EXIT_FAILURE);
        }
        
    } while (fabs(x - x_prev) > EPSILON);
    
    return x;
}

int main(void) {
    double guess = safe_input("Enter initial guess for Newton-Raphson method: ");
    
    if (fabs(guess) > MAX_INPUT) {
        fprintf(stderr, "Initial guess out of range\n");
        return EXIT_FAILURE;
    }
    
    double root = newton_raphson(guess);
    
    printf("Approximate root: %.10f\n", root);
    printf("Function value at root: %.10f\n", root * root * root - 2.0 * root - 5.0);
    
    return EXIT_SUCCESS;
}