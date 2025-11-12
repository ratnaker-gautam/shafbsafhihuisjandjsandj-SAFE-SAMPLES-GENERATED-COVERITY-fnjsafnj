//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <float.h>

#define MAX_ITERATIONS 1000000

int main() {
    int n;
    printf("Enter number of terms to compute (1-%d): ", MAX_ITERATIONS);
    
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (n < 1 || n > MAX_ITERATIONS) {
        fprintf(stderr, "Error: Number out of range\n");
        return 1;
    }
    
    double sum = 0.0;
    double term;
    int sign = 1;
    
    for (int i = 1; i <= n; i++) {
        if (i > INT_MAX / 2) {
            fprintf(stderr, "Error: Integer overflow detected\n");
            return 1;
        }
        
        int denominator = 2 * i - 1;
        if (denominator == 0) {
            fprintf(stderr, "Error: Division by zero\n");
            return 1;
        }
        
        term = 1.0 / denominator;
        if (sign > 0) {
            if (sum > DBL_MAX - term) {
                fprintf(stderr, "Error: Floating point overflow\n");
                return 1;
            }
            sum += term;
        } else {
            if (sum < -DBL_MAX + term) {
                fprintf(stderr, "Error: Floating point overflow\n");
                return 1;
            }
            sum -= term;
        }
        
        sign = -sign;
    }
    
    double pi_approx = 4.0 * sum;
    printf("Approximation of π using %d terms: %.15f\n", n, pi_approx);
    
    return 0;
}