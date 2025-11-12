//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define MAX_INPUT 1000000

int main() {
    int n;
    double sum = 0.0;
    double product = 1.0;
    
    printf("Enter a positive integer (1-%d): ", MAX_INPUT);
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (n < 1 || n > MAX_INPUT) {
        fprintf(stderr, "Error: Number out of range\n");
        return 1;
    }
    
    printf("Computing sum and product of reciprocals...\n");
    
    int i = 1;
    while (i <= n) {
        double reciprocal = 1.0 / i;
        sum += reciprocal;
        
        if (product * reciprocal > 1e308) {
            fprintf(stderr, "Error: Product overflow\n");
            return 1;
        }
        product *= reciprocal;
        
        i++;
    }
    
    printf("Results for n = %d:\n", n);
    printf("Sum of reciprocals: %.10f\n", sum);
    printf("Product of reciprocals: %.10e\n", product);
    
    double harmonic_mean = n / sum;
    double geometric_mean = pow(product, 1.0 / n);
    
    printf("Harmonic mean: %.10f\n", harmonic_mean);
    printf("Geometric mean: %.10f\n", geometric_mean);
    
    double ratio = harmonic_mean / geometric_mean;
    printf("Ratio (harmonic/geometric): %.10f\n", ratio);
    
    return 0;
}