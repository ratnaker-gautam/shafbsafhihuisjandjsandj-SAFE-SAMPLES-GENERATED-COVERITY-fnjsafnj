//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10

double f(double x) {
    return x*x*x - 2*x - 5;
}

double f_prime(double x) {
    return 3*x*x - 2;
}

int main(void) {
    double x0, x1;
    int iterations = 0;
    
    printf("Enter initial guess for root: ");
    if (scanf("%lf", &x0) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return EXIT_FAILURE;
    }
    
    if (fabs(f_prime(x0)) < EPSILON) {
        fprintf(stderr, "Error: Initial guess leads to zero derivative\n");
        return EXIT_FAILURE;
    }
    
    printf("Iteration %2d: x = %.10f, f(x) = %.10f\n", iterations, x0, f(x0));
    
    while (iterations < MAX_ITERATIONS) {
        double derivative = f_prime(x0);
        if (fabs(derivative) < EPSILON) {
            fprintf(stderr, "Error: Derivative too close to zero\n");
            return EXIT_FAILURE;
        }
        
        x1 = x0 - f(x0) / derivative;
        
        iterations++;
        printf("Iteration %2d: x = %.10f, f(x) = %.10f\n", iterations, x1, f(x1));
        
        if (fabs(x1 - x0) < EPSILON && fabs(f(x1)) < EPSILON) {
            printf("\nConverged to root: %.10f\n", x1);
            printf("Function value at root: %.10f\n", f(x1));
            printf("Iterations required: %d\n", iterations);
            return EXIT_SUCCESS;
        }
        
        x0 = x1;
    }
    
    fprintf(stderr, "Error: Failed to converge within %d iterations\n", MAX_ITERATIONS);
    return EXIT_FAILURE;
}