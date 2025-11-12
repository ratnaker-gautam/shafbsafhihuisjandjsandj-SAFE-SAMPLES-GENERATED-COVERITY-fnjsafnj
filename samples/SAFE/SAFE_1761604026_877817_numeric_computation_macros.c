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

double compute_polynomial(double x) {
    return 2.0 * SQUARE(x) - 3.0 * x + 1.5;
}

double compute_derivative(double x) {
    return 4.0 * x - 3.0;
}

double newton_raphson(double initial_guess) {
    double x = initial_guess;
    int iterations = 0;
    
    while (iterations < MAX_ITERATIONS) {
        double fx = compute_polynomial(x);
        double dfx = compute_derivative(x);
        
        if (fabs(dfx) < EPSILON) {
            break;
        }
        
        double delta = fx / dfx;
        if (fabs(delta) < EPSILON) {
            break;
        }
        
        x = x - delta;
        iterations++;
    }
    
    return x;
}

int main(void) {
    double initial_guess;
    char input_buffer[256];
    
    printf("Enter initial guess for root finding (-1000000 to 1000000): ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    char *endptr;
    initial_guess = strtod(input_buffer, &endptr);
    
    if (endptr == input_buffer || *endptr != '\n') {
        fprintf(stderr, "Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    if (!VALIDATE_INPUT(initial_guess)) {
        fprintf(stderr, "Input out of valid range\n");
        return EXIT_FAILURE;
    }
    
    if (isnan(initial_guess) || isinf(initial_guess)) {
        fprintf(stderr, "Input is not a finite number\n");
        return EXIT_FAILURE;
    }
    
    double root = newton_raphson(initial_guess);
    
    if (isnan(root) || isinf(root)) {
        fprintf(stderr, "Computation failed to converge\n");
        return EXIT_FAILURE;
    }
    
    printf("Approximate root: %.10f\n", root);
    printf("Function value at root: %.10f\n", compute_polynomial(root));
    
    return EXIT_SUCCESS;
}