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
            printf("Input out of range. Please enter a value between %.0f and %.0f.\n", MIN_INPUT, MAX_INPUT);
            continue;
        }
        
        if (value != value) {
            printf("Invalid input. Please enter a valid number.\n");
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
        
        if (fabs(2 * x) < EPSILON) {
            fprintf(stderr, "Division by zero avoided\n");
            return NAN;
        }
        
        double fx = x * x - 2.0;
        double fpx = 2.0 * x;
        x = x - fx / fpx;
        
        iterations++;
        if (iterations > MAX_ITERATIONS) {
            fprintf(stderr, "Maximum iterations reached\n");
            return NAN;
        }
        
        if (x != x) {
            fprintf(stderr, "Numerical instability detected\n");
            return NAN;
        }
        
    } while (fabs(x - x_prev) > EPSILON && iterations < MAX_ITERATIONS);
    
    return x;
}

int main(void) {
    printf("Newton-Raphson Method for finding square root of 2\n");
    printf("==================================================\n");
    
    double guess = safe_input("Enter initial guess: ");
    
    if (guess == 0.0) {
        fprintf(stderr, "Initial guess cannot be zero\n");
        return EXIT_FAILURE;
    }
    
    double result = newton_raphson(guess);
    
    if (result != result) {
        fprintf(stderr, "Computation failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Approximated square root of 2: %.10f\n", result);
    printf("Actual square root of 2: %.10f\n", sqrt(2.0));
    printf("Absolute error: %.10f\n", fabs(result - sqrt(2.0)));
    
    return EXIT_SUCCESS;
}