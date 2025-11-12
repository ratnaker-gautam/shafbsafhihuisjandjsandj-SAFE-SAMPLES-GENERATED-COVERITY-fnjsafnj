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
    char buffer[256];
    
    printf("Enter initial guess for Newton-Raphson method: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (sscanf(buffer, "%lf", &x0) != 1) {
        fprintf(stderr, "Invalid input. Please enter a valid number.\n");
        return 1;
    }
    
    if (fabs(f_prime(x0)) < EPSILON) {
        fprintf(stderr, "Initial guess leads to division by zero. Try a different value.\n");
        return 1;
    }
    
    printf("Iteration %2d: x = %.10f, f(x) = %.10f\n", iterations, x0, f(x0));
    
    while (iterations < MAX_ITERATIONS) {
        double derivative = f_prime(x0);
        if (fabs(derivative) < EPSILON) {
            fprintf(stderr, "Derivative too small. Method failed.\n");
            return 1;
        }
        
        x1 = x0 - f(x0) / derivative;
        
        if (fabs(x1 - x0) < EPSILON) {
            printf("Converged after %d iterations\n", iterations + 1);
            printf("Root found at x = %.10f\n", x1);
            printf("f(x) = %.10f\n", f(x1));
            return 0;
        }
        
        x0 = x1;
        iterations++;
        printf("Iteration %2d: x = %.10f, f(x) = %.10f\n", iterations, x0, f(x0));
    }
    
    fprintf(stderr, "Method did not converge within %d iterations\n", MAX_ITERATIONS);
    return 1;
}