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
        product *= reciprocal;
        i++;
    }
    
    double harmonic_mean = n / sum;
    double geometric_mean = pow(product, 1.0 / n);
    
    printf("Results for n = %d:\n", n);
    printf("Harmonic mean: %.6f\n", harmonic_mean);
    printf("Geometric mean: %.6f\n", geometric_mean);
    
    double ratio = harmonic_mean / geometric_mean;
    printf("Ratio (H/G): %.6f\n", ratio);
    
    int j;
    double running_sum = 0.0;
    printf("Partial sums: ");
    for (j = 1; j <= n && j <= 10; j++) {
        running_sum += 1.0 / j;
        printf("%.3f", running_sum);
        if (j < n && j < 10) {
            printf(", ");
        }
    }
    printf("\n");
    
    int k = n;
    double reverse_sum = 0.0;
    do {
        if (k >= 1) {
            reverse_sum += 1.0 / k;
        }
        k--;
    } while (k >= 1);
    
    printf("Reverse sum: %.6f\n", reverse_sum);
    
    return 0;
}