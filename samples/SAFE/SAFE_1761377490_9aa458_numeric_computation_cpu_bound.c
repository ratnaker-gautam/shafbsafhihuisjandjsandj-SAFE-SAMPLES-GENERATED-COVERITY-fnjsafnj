//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <math.h>

#define MAX_ITERATIONS 1000000

int main() {
    int n;
    printf("Enter number of iterations (1-%d): ", MAX_ITERATIONS);
    
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (n < 1 || n > MAX_ITERATIONS) {
        fprintf(stderr, "Error: Number out of range\n");
        return 1;
    }
    
    double pi_approximation = 0.0;
    int sign = 1;
    
    for (int i = 0; i < n; i++) {
        if (i > INT_MAX / 2) {
            fprintf(stderr, "Error: Integer overflow detected\n");
            return 1;
        }
        
        double denominator = 2.0 * i + 1.0;
        if (denominator == 0.0) {
            fprintf(stderr, "Error: Division by zero\n");
            return 1;
        }
        
        double term = sign / denominator;
        
        if (isinf(term) || isnan(term)) {
            fprintf(stderr, "Error: Numerical overflow detected\n");
            return 1;
        }
        
        pi_approximation += term;
        sign = -sign;
    }
    
    pi_approximation *= 4.0;
    
    if (isinf(pi_approximation) || isnan(pi_approximation)) {
        fprintf(stderr, "Error: Final result overflow\n");
        return 1;
    }
    
    double actual_pi = 3.14159265358979323846;
    printf("Approximation of pi after %d iterations: %.15f\n", n, pi_approximation);
    printf("Difference from actual pi: %.15f\n", fabs(pi_approximation - actual_pi));
    
    return 0;
}