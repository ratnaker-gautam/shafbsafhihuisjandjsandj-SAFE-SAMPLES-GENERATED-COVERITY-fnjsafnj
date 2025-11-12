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
        fprintf(stderr, "Error: Input out of range\n");
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
    
    double geometric_mean = 1.0;
    for (int m = 1; m <= n; m++) {
        geometric_mean *= m;
    }
    geometric_mean = pow(geometric_mean, 1.0 / n);
    
    double arithmetic_mean = sum / n;
    
    printf("Results for n = %d:\n", n);
    printf("Sum (1 to n): %.0f\n", sum);
    printf("Product (1 to n): %.0f\n", product);
    printf("Harmonic mean: %f\n", n / harmonic);
    printf("Geometric mean: %f\n", geometric_mean);
    printf("Arithmetic mean: %f\n", arithmetic_mean);
    
    double ratio = geometric_mean / arithmetic_mean;
    printf("Ratio (geometric/arithmetic): %f\n", ratio);
    
    int iter_count = 0;
    double current = 1.0;
    double prev_ratio = 0.0;
    
    while (iter_count < MAX_ITERATIONS) {
        prev_ratio = ratio;
        ratio = sqrt(ratio * prev_ratio);
        iter_count++;
        
        if (fabs(ratio - prev_ratio) < 1e-10) {
            break;
        }
    }
    
    printf("Converged ratio after %d iterations: %f\n", iter_count, ratio);
    
    return 0;
}