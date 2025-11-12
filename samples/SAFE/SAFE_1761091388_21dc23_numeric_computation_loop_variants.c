//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000

int main(void) {
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
        term = 1.0 / (i + 1);
        if (i % 2 == 1) {
            term = -term;
        }
        sum += term;
        printf("Term %d: %+.6f, Partial sum: %.6f\n", i + 1, term, sum);
        i++;
    }
    
    printf("\n");
    
    double product = 1.0;
    int j;
    
    for (j = 1; j <= n; j++) {
        double factor = 1.0 + (1.0 / (j * j));
        if (factor > 0.0) {
            product *= factor;
            printf("Factor %d: %.6f, Partial product: %.6f\n", j, factor, product);
        }
    }
    
    printf("\n");
    
    double power_sum = 0.0;
    int k = 0;
    
    do {
        if (k >= n) break;
        
        double value = (double)(k + 1);
        double powered = value * value;
        
        if (powered > 0.0 && powered < 1e100) {
            power_sum += powered;
            printf("Square %d: %.1f, Power sum: %.1f\n", k + 1, powered, power_sum);
        }
        k++;
    } while (k < n);
    
    printf("\nFinal alternating harmonic sum: %.6f\n", sum);
    printf("Final product: %.6f\n", product);
    printf("Final sum of squares: %.1f\n", power_sum);
    
    return 0;
}