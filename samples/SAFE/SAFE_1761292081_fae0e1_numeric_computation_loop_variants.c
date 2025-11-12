//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <float.h>

#define MAX_ITERATIONS 1000

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
    int i = 0;
    
    printf("Computing series approximation of e^x at x=1:\n");
    
    while (i < n) {
        double factorial = 1.0;
        int j = 1;
        
        while (j <= i) {
            if (factorial > DBL_MAX / j) {
                fprintf(stderr, "Error: Factorial overflow\n");
                return 1;
            }
            factorial *= j;
            j++;
        }
        
        term = 1.0 / factorial;
        if (sum > DBL_MAX - term) {
            fprintf(stderr, "Error: Sum overflow\n");
            return 1;
        }
        sum += term;
        
        printf("Term %d: 1/%d! = %.10f\n", i+1, i, term);
        i++;
    }
    
    printf("\nFinal approximation: %.15f\n", sum);
    printf("Actual e value: 2.718281828459045\n");
    
    double error = sum - 2.718281828459045;
    if (error < 0) error = -error;
    printf("Absolute error: %.15f\n", error);
    
    return 0;
}