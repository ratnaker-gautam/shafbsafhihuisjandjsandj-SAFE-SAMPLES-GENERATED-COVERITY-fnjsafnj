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
    
    printf("Computing sum and product using different loop variants...\n");
    
    int i = 1;
    while (i <= n) {
        sum += i;
        i++;
    }
    
    for (int j = 1; j <= n; j++) {
        if (product > 1e300) {
            fprintf(stderr, "Error: Product too large\n");
            return 1;
        }
        product *= j;
    }
    
    double harmonic = 0.0;
    int k = 1;
    do {
        harmonic += 1.0 / k;
        k++;
    } while (k <= n);
    
    double geometric_sum = 0.0;
    double ratio = 0.5;
    double term = 1.0;
    for (int m = 0; m < n; m++) {
        geometric_sum += term;
        term *= ratio;
        if (geometric_sum > 1e300) {
            fprintf(stderr, "Error: Geometric sum too large\n");
            return 1;
        }
    }
    
    printf("Sum of first %d integers: %.0f\n", n, sum);
    printf("Product of first %d integers: %.0f\n", n, product);
    printf("Harmonic sum of first %d integers: %f\n", n, harmonic);
    printf("Geometric sum with ratio 0.5 for %d terms: %f\n", n, geometric_sum);
    
    double root_mean_square = 0.0;
    for (int p = 1; p <= n; p++) {
        root_mean_square += p * p;
    }
    root_mean_square = sqrt(root_mean_square / n);
    printf("Root mean square of first %d integers: %f\n", n, root_mean_square);
    
    return 0;
}