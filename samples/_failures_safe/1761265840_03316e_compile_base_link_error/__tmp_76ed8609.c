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
        sum += sqrt(i);
        i++;
    }
    
    for (int j = 1; j <= n; j++) {
        product *= 1.0 + (1.0 / j);
    }
    
    double harmonic_sum = 0.0;
    int k = 1;
    do {
        harmonic_sum += 1.0 / k;
        k++;
    } while (k <= n);
    
    double geometric_sum = 0.0;
    double ratio = 0.5;
    int m;
    for (m = 0; m < n; m++) {
        geometric_sum += pow(ratio, m);
    }
    
    printf("Results for n = %d:\n", n);
    printf("Sum of square roots: %.6f\n", sum);
    printf("Product of (1 + 1/k): %.6f\n", product);
    printf("Harmonic sum: %.6f\n", harmonic_sum);
    printf("Geometric sum (ratio=0.5): %.6f\n", geometric_sum);
    
    if (n > 1) {
        double variance = 0.0;
        double mean = sum / n;
        
        int idx = 1;
        while (idx <= n) {
            double diff = sqrt(idx) - mean;
            variance += diff * diff;
            idx++;
        }
        variance /= n;
        
        printf("Variance of square roots: %.6f\n", variance);
    }
    
    return 0;
}