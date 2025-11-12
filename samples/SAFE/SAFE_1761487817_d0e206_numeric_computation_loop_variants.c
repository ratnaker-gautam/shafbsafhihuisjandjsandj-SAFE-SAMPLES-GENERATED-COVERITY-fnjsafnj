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
        fprintf(stderr, "Error: Number must be between 1 and %d\n", MAX_ITERATIONS);
        return 1;
    }
    
    double sum = 0.0;
    double term;
    int i = 0;
    
    printf("Computing alternating harmonic series for %d terms:\n", n);
    
    while (i < n) {
        if (i + 1 == 0) {
            fprintf(stderr, "Error: Division by zero\n");
            return 1;
        }
        term = 1.0 / (i + 1);
        if (i % 2 == 1) {
            term = -term;
        }
        sum += term;
        printf("Term %d: %f, Partial sum: %f\n", i + 1, term, sum);
        i++;
    }
    
    printf("\n");
    printf("Final sum: %f\n", sum);
    
    double product = 1.0;
    printf("\nComputing Wallis product for %d terms:\n", n);
    
    for (int j = 1; j <= n; j++) {
        double numerator = 4.0 * j * j;
        double denominator = (4.0 * j * j) - 1.0;
        if (denominator == 0.0) {
            fprintf(stderr, "Error: Division by zero\n");
            return 1;
        }
        product *= numerator / denominator;
        printf("Term %d: %f, Partial product: %f\n", j, numerator / denominator, product);
    }
    
    printf("Final product: %f\n", product);
    
    double factorial_sum = 0.0;
    printf("\nComputing sum of reciprocal factorials for %d terms:\n", n);
    
    int k = 0;
    double current_factorial = 1.0;
    
    do {
        if (k > 0) {
            if (current_factorial > DBL_MAX / k) {
                fprintf(stderr, "Warning: Factorial overflow at term %d\n", k + 1);
                break;
            }
            current_factorial *= k;
        }
        
        if (current_factorial == 0.0) {
            fprintf(stderr, "Error: Division by zero\n");
            return 1;
        }
        double reciprocal = 1.0 / current_factorial;
        factorial_sum += reciprocal;
        printf("Term %d: %e, Partial sum: %f\n", k + 1, reciprocal, factorial_sum);
        k++;
    } while (k < n);
    
    printf("Final factorial sum: %f\n", factorial_sum);
    
    return 0;
}