//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define TOLERANCE 1e-10

double compute_function(double x) {
    return x*x*x - 2*x*x - 5*x + 6;
}

double compute_derivative(double x) {
    return 3*x*x - 4*x - 5;
}

int main(void) {
    double initial_guess;
    char input_buffer[256];
    
    printf("Enter initial guess for root finding: ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (sscanf(input_buffer, "%lf", &initial_guess) != 1) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    if (!isfinite(initial_guess)) {
        fprintf(stderr, "Input value is not finite\n");
        return 1;
    }
    
    double x = initial_guess;
    double fx = compute_function(x);
    double dfx;
    int iteration;
    
    printf("Iteration: %3d, x = %12.8f, f(x) = %12.8f\n", 0, x, fx);
    
    for (iteration = 1; iteration <= MAX_ITERATIONS; iteration++) {
        dfx = compute_derivative(x);
        
        if (fabs(dfx) < TOLERANCE) {
            fprintf(stderr, "Derivative too small, method fails\n");
            return 1;
        }
        
        double delta = fx / dfx;
        
        if (!isfinite(delta)) {
            fprintf(stderr, "Numerical instability detected\n");
            return 1;
        }
        
        double x_new = x - delta;
        
        if (!isfinite(x_new)) {
            fprintf(stderr, "Numerical overflow detected\n");
            return 1;
        }
        
        double fx_new = compute_function(x_new);
        
        printf("Iteration: %3d, x = %12.8f, f(x) = %12.8f\n", iteration, x_new, fx_new);
        
        if (fabs(fx_new) < TOLERANCE) {
            printf("\nRoot found: x = %.10f\n", x_new);
            printf("Function value at root: %.2e\n", fx_new);
            printf("Converged in %d iterations\n", iteration);
            return 0;
        }
        
        if (fabs(x_new - x) < TOLERANCE) {
            printf("\nApproximate root: x = %.10f\n", x_new);
            printf("Function value: %.2e\n", fx_new);
            printf("Converged by step size in %d iterations\n", iteration);
            return 0;
        }
        
        x = x_new;
        fx = fx_new;
    }
    
    fprintf(stderr, "Failed to converge within %d iterations\n", MAX_ITERATIONS);
    return 1;
}