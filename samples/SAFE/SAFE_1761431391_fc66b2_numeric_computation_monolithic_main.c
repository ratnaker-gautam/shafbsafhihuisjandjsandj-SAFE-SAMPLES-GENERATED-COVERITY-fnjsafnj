//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10

double f(double x) {
    return x*x*x - 2*x*x - 5*x + 6;
}

double f_prime(double x) {
    return 3*x*x - 4*x - 5;
}

int main(void) {
    double x0, x1;
    int iterations = 0;
    
    printf("Enter initial guess for Newton-Raphson method: ");
    if (scanf("%lf", &x0) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return EXIT_FAILURE;
    }
    
    if (!isfinite(x0)) {
        fprintf(stderr, "Error: Invalid initial guess\n");
        return EXIT_FAILURE;
    }
    
    printf("Iteration %2d: x = %.10f, f(x) = %.10f\n", iterations, x0, f(x0));
    
    while (iterations < MAX_ITERATIONS) {
        double fx = f(x0);
        double fpx = f_prime(x0);
        
        if (fabs(fpx) < EPSILON) {
            fprintf(stderr, "Error: Derivative too small\n");
            return EXIT_FAILURE;
        }
        
        if (!isfinite(fx) || !isfinite(fpx)) {
            fprintf(stderr, "Error: Function evaluation failed\n");
            return EXIT_FAILURE;
        }
        
        x1 = x0 - fx / fpx;
        
        if (!isfinite(x1)) {
            fprintf(stderr, "Error: Numerical overflow\n");
            return EXIT_FAILURE;
        }
        
        iterations++;
        printf("Iteration %2d: x = %.10f, f(x) = %.10f\n", iterations, x1, f(x1));
        
        if (fabs(x1 - x0) < EPSILON) {
            printf("\nRoot found: x = %.10f\n", x1);
            printf("f(x) = %.10f\n", f(x1));
            printf("Iterations: %d\n", iterations);
            return EXIT_SUCCESS;
        }
        
        x0 = x1;
    }
    
    fprintf(stderr, "Error: Maximum iterations reached\n");
    return EXIT_FAILURE;
}