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
    
    if (n <= 0 || n > MAX_INPUT) {
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
        if (product > 1e308 / j) {
            fprintf(stderr, "Error: Product would overflow\n");
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
    }
    
    double root_mean_square = 0.0;
    for (int p = 1; p <= n; p++) {
        root_mean_square += p * p;
    }
    root_mean_square = sqrt(root_mean_square / n);
    
    printf("Sum of first %d integers: %.0f\n", n, sum);
    printf("Product of first %d integers: %.0f\n", n, product);
    printf("Harmonic sum of first %d integers: %.6f\n", n, harmonic);
    printf("Geometric sum with ratio 0.5 for %d terms: %.6f\n", n, geometric_sum);
    printf("Root mean square of first %d integers: %.6f\n", n, root_mean_square);
    
    return 0;
}