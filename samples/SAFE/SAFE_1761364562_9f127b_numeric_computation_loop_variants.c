//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <float.h>

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
        if (i == INT_MAX) {
            fprintf(stderr, "Error: Integer overflow\n");
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
    
    double product = 1.0;
    int j;
    
    for (j = 1; j <= n; j++) {
        if (j == 0) {
            fprintf(stderr, "Error: Division by zero\n");
            return 1;
        }
        double factor = 1.0 + (1.0 / (j * j));
        if (factor > 0 && product < (DBL_MAX / factor)) {
            product *= factor;
        } else {
            fprintf(stderr, "Error: Numerical overflow detected\n");
            return 1;
        }
        printf("Factor %d: %f, Partial product: %f\n", j, factor, product);
    }
    
    printf("\n");
    
    double geometric_sum = 0.0;
    double ratio = 0.5;
    double current_term = 1.0;
    int k = 0;
    
    do {
        geometric_sum += current_term;
        printf("Geometric term %d: %f, Partial sum: %f\n", k + 1, current_term, geometric_sum);
        current_term *= ratio;
        k++;
        if (k >= MAX_ITERATIONS) {
            break;
        }
    } while (k < n && current_term > 1e-15);
    
    printf("\nFinal alternating harmonic sum: %f\n", sum);
    printf("Final product: %f\n", product);
    printf("Final geometric sum: %f\n", geometric_sum);
    
    return 0;
}