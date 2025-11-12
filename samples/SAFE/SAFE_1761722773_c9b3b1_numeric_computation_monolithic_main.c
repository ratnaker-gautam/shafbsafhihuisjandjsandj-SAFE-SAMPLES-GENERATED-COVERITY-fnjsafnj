//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define TOLERANCE 1e-10

double f(double x) {
    return x*x*x - 2*x*x - 5*x + 6;
}

double f_prime(double x) {
    return 3*x*x - 4*x - 5;
}

int main(void) {
    double x0, x1, fx0, fpx0, error;
    int iterations = 0;
    
    printf("Enter initial guess for root: ");
    if (scanf("%lf", &x0) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return EXIT_FAILURE;
    }
    
    if (!isfinite(x0)) {
        fprintf(stderr, "Error: Invalid initial guess\n");
        return EXIT_FAILURE;
    }
    
    printf("Iteration %2d: x = %.10f, f(x) = %.10f\n", iterations, x0, f(x0));
    
    do {
        fx0 = f(x0);
        fpx0 = f_prime(x0);
        
        if (fabs(fpx0) < TOLERANCE) {
            fprintf(stderr, "Error: Derivative too small\n");
            return EXIT_FAILURE;
        }
        
        x1 = x0 - fx0 / fpx0;
        
        if (!isfinite(x1)) {
            fprintf(stderr, "Error: Numerical overflow\n");
            return EXIT_FAILURE;
        }
        
        error = fabs(x1 - x0);
        x0 = x1;
        iterations++;
        
        printf("Iteration %2d: x = %.10f, f(x) = %.10f, error = %.10f\n", 
               iterations, x0, f(x0), error);
        
        if (iterations >= MAX_ITERATIONS) {
            fprintf(stderr, "Error: Maximum iterations reached\n");
            return EXIT_FAILURE;
        }
        
    } while (error > TOLERANCE && fabs(f(x0)) > TOLERANCE);
    
    printf("\nRoot found: x = %.10f\n", x0);
    printf("f(x) = %.10f\n", f(x0));
    printf("Iterations: %d\n", iterations);
    
    return EXIT_SUCCESS;
}