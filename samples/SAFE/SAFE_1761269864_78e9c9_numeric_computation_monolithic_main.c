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

int main() {
    double x0, x1;
    int iterations = 0;
    
    printf("Enter initial guess for root: ");
    if (scanf("%lf", &x0) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (fabs(f_prime(x0)) < EPSILON) {
        fprintf(stderr, "Error: Initial guess leads to zero derivative\n");
        return 1;
    }
    
    printf("Iteration %2d: x = %.10f, f(x) = %.10f\n", iterations, x0, f(x0));
    
    while (iterations < MAX_ITERATIONS) {
        double derivative = f_prime(x0);
        if (fabs(derivative) < EPSILON) {
            fprintf(stderr, "Error: Derivative too small\n");
            return 1;
        }
        
        x1 = x0 - f(x0) / derivative;
        
        iterations++;
        printf("Iteration %2d: x = %.10f, f(x) = %.10f\n", iterations, x1, f(x1));
        
        if (fabs(x1 - x0) < EPSILON || fabs(f(x1)) < EPSILON) {
            printf("\nRoot found: x = %.10f\n", x1);
            printf("f(x) = %.10f\n", f(x1));
            printf("Iterations: %d\n", iterations);
            return 0;
        }
        
        x0 = x1;
    }
    
    fprintf(stderr, "Error: Maximum iterations reached\n");
    return 1;
}