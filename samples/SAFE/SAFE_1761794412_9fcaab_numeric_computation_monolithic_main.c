//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define TOLERANCE 1e-10

double f(double x) {
    return x*x*x - 2*x - 5;
}

double f_prime(double x) {
    return 3*x*x - 2;
}

int main() {
    double x0, x1;
    int iterations = 0;
    
    printf("Enter initial guess for root: ");
    if (scanf("%lf", &x0) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (fabs(f_prime(x0)) < 1e-15) {
        fprintf(stderr, "Error: Derivative too small\n");
        return 1;
    }
    
    printf("Iterating to find root of x^3 - 2x - 5 = 0\n");
    printf("Initial guess: %.6f\n", x0);
    
    while (iterations < MAX_ITERATIONS) {
        double fx = f(x0);
        double fpx = f_prime(x0);
        
        if (fabs(fpx) < 1e-15) {
            fprintf(stderr, "Error: Derivative became zero\n");
            return 1;
        }
        
        x1 = x0 - fx / fpx;
        
        if (fabs(x1 - x0) < TOLERANCE) {
            printf("Converged after %d iterations\n", iterations + 1);
            printf("Root found at: %.10f\n", x1);
            printf("Function value at root: %.10f\n", f(x1));
            return 0;
        }
        
        x0 = x1;
        iterations++;
    }
    
    fprintf(stderr, "Error: Failed to converge after %d iterations\n", MAX_ITERATIONS);
    printf("Last approximation: %.10f\n", x1);
    printf("Function value: %.10f\n", f(x1));
    return 1;
}