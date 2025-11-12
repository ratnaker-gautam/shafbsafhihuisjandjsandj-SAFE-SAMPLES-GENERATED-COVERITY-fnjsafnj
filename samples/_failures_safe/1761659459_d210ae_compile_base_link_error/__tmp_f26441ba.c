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
        if (j > 0 && product > 0 && j <= MAX_INPUT / product) {
            product *= j;
        } else {
            fprintf(stderr, "Error: Product overflow\n");
            return 1;
        }
    }
    
    double harmonic_mean = 0.0;
    int k = 1;
    do {
        harmonic_mean += 1.0 / k;
        k++;
    } while (k <= n);
    
    if (harmonic_mean > 0) {
        harmonic_mean = n / harmonic_mean;
    } else {
        fprintf(stderr, "Error: Harmonic mean calculation failed\n");
        return 1;
    }
    
    double geometric_mean;
    if (product > 0 && n > 0) {
        geometric_mean = pow(product, 1.0 / n);
    } else {
        fprintf(stderr, "Error: Geometric mean calculation failed\n");
        return 1;
    }
    
    double arithmetic_mean = sum / n;
    
    printf("Results for n = %d:\n", n);
    printf("Sum (1 to n): %.0f\n", sum);
    printf("Product (1 to n): %.0f\n", product);
    printf("Arithmetic mean: %.6f\n", arithmetic_mean);
    printf("Geometric mean: %.6f\n", geometric_mean);
    printf("Harmonic mean: %.6f\n", harmonic_mean);
    
    return 0;
}