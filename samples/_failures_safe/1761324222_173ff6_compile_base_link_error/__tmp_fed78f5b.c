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
        sum += sqrt(i);
        i++;
    }
    
    for (int j = 1; j <= n; j++) {
        if (j > 0 && product < __DBL_MAX__ / j) {
            product *= j;
        } else {
            fprintf(stderr, "Error: Product overflow\n");
            return 1;
        }
    }
    
    double harmonic = 0.0;
    int k = 1;
    do {
        harmonic += 1.0 / k;
        k++;
    } while (k <= n);
    
    double geometric_mean = 0.0;
    for (int m = n; m >= 1; m--) {
        geometric_mean += log(m);
    }
    geometric_mean = exp(geometric_mean / n);
    
    printf("Results for n = %d:\n", n);
    printf("Sum of square roots: %.6f\n", sum);
    printf("Factorial product: %.0f\n", product);
    printf("Harmonic mean: %.6f\n", harmonic);
    printf("Geometric mean: %.6f\n", geometric_mean);
    
    double final_result = (sum + product + harmonic + geometric_mean) / 4.0;
    printf("Final combined result: %.6f\n", final_result);
    
    return 0;
}