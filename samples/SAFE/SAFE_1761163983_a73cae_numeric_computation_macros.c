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

double newton_raphson(double initial_guess, double target);
double polynomial(double x);
double derivative(double x);

int main() {
    double target_value, initial_guess;
    char input_buffer[256];
    
    printf("Enter target value for polynomial root finding (-1000000 to 1000000): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    if (sscanf(input_buffer, "%lf", &target_value) != 1) {
        fprintf(stderr, "Invalid numeric input\n");
        return EXIT_FAILURE;
    }
    
    if (!VALIDATE_INPUT(target_value)) {
        fprintf(stderr, "Target value out of range\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter initial guess (-1000000 to 1000000): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    if (sscanf(input_buffer, "%lf", &initial_guess) != 1) {
        fprintf(stderr, "Invalid numeric input\n");
        return EXIT_FAILURE;
    }
    
    if (!VALIDATE_INPUT(initial_guess)) {
        fprintf(stderr, "Initial guess out of range\n");
        return EXIT_FAILURE;
    }
    
    double result = newton_raphson(initial_guess, target_value);
    
    if (isnan(result)) {
        printf("Failed to converge to a solution\n");
        return EXIT_FAILURE;
    }
    
    printf("Found root: %.10f\n", result);
    printf("Polynomial value at root: %.10f\n", polynomial(result) - target_value);
    
    return EXIT_SUCCESS;
}

double polynomial(double x) {
    return CUBE(x) - 2.0 * SQUARE(x) - 5.0 * x + 6.0;
}

double derivative(double x) {
    return 3.0 * SQUARE(x) - 4.0 * x - 5.0;
}

double newton_raphson(double initial_guess, double target) {
    double x = initial_guess;
    int iterations = 0;
    
    while (iterations < MAX_ITERATIONS) {
        double fx = polynomial(x) - target;
        double dfx = derivative(x);
        
        if (fabs(dfx) < EPSILON) {
            return NAN;
        }
        
        double delta = fx / dfx;
        
        if (fabs(delta) < EPSILON) {
            break;
        }
        
        double new_x = x - delta;
        
        if (!VALIDATE_INPUT(new_x)) {
            return NAN;
        }
        
        x = new_x;
        iterations++;
    }
    
    if (iterations >= MAX_ITERATIONS) {
        return NAN;
    }
    
    return x;
}