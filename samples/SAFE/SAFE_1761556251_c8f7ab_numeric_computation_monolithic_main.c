//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define TOLERANCE 1e-10

double compute_function(double x) {
    return x * x * x - 2 * x * x - 5 * x + 6;
}

double compute_derivative(double x) {
    return 3 * x * x - 4 * x - 5;
}

int main(void) {
    double initial_guess;
    double x, x_new, fx, dfx;
    int iterations = 0;
    char input_buffer[256];
    
    printf("Newton-Raphson Method for finding roots\n");
    printf("Function: f(x) = x^3 - 2x^2 - 5x + 6\n");
    printf("Enter initial guess: ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (sscanf(input_buffer, "%lf", &initial_guess) != 1) {
        fprintf(stderr, "Invalid input. Please enter a valid number.\n");
        return 1;
    }
    
    if (!isfinite(initial_guess)) {
        fprintf(stderr, "Invalid initial guess\n");
        return 1;
    }
    
    x = initial_guess;
    
    printf("\nIteration  x          f(x)        f'(x)\n");
    printf("----------------------------------------\n");
    
    while (iterations < MAX_ITERATIONS) {
        fx = compute_function(x);
        dfx = compute_derivative(x);
        
        printf("%9d  %10.6f  %10.6f  %10.6f\n", iterations, x, fx, dfx);
        
        if (fabs(dfx) < TOLERANCE) {
            fprintf(stderr, "Derivative too small. Method failed.\n");
            return 1;
        }
        
        if (fabs(fx) < TOLERANCE) {
            printf("\nRoot found: x = %.10f\n", x);
            printf("f(x) = %.10f\n", fx);
            printf("Converged in %d iterations\n", iterations);
            return 0;
        }
        
        x_new = x - fx / dfx;
        
        if (!isfinite(x_new)) {
            fprintf(stderr, "Numerical instability detected\n");
            return 1;
        }
        
        if (fabs(x_new - x) < TOLERANCE) {
            printf("\nRoot found: x = %.10f\n", x_new);
            printf("f(x) = %.10f\n", compute_function(x_new));
            printf("Converged in %d iterations\n", iterations + 1);
            return 0;
        }
        
        x = x_new;
        iterations++;
    }
    
    fprintf(stderr, "Maximum iterations reached without convergence\n");
    printf("Best approximation: x = %.10f\n", x);
    printf("f(x) = %.10f\n", compute_function(x));
    
    return 1;
}