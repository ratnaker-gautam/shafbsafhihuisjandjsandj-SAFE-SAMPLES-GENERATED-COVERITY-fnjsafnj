//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define ARRAY_SIZE 100

int main() {
    int n;
    printf("Enter number of terms to compute (1-%d): ", ARRAY_SIZE);
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (n < 1 || n > ARRAY_SIZE) {
        fprintf(stderr, "Error: Number must be between 1 and %d\n", ARRAY_SIZE);
        return 1;
    }
    
    double sequence[ARRAY_SIZE];
    double sum = 0.0;
    double product = 1.0;
    
    printf("Computing sequence terms...\n");
    
    int i = 0;
    while (i < n) {
        double term = sin((i + 1) * 0.1);
        if (isnan(term) || isinf(term)) {
            fprintf(stderr, "Error: Numerical overflow in computation\n");
            return 1;
        }
        sequence[i] = term;
        i++;
    }
    
    for (int j = 0; j < n; j++) {
        sum += sequence[j];
        if (isinf(sum) || isnan(sum)) {
            fprintf(stderr, "Error: Sum overflow\n");
            return 1;
        }
    }
    
    int k = 0;
    do {
        if (k >= n) break;
        product *= sequence[k];
        if (isinf(product) || isnan(product) || fabs(product) > 1e300) {
            fprintf(stderr, "Error: Product overflow\n");
            return 1;
        }
        k++;
    } while (k < n);
    
    double harmonic_mean;
    if (sum != 0.0) {
        harmonic_mean = n / sum;
    } else {
        harmonic_mean = 0.0;
    }
    
    double geometric_mean;
    if (product >= 0.0) {
        geometric_mean = pow(product, 1.0 / n);
    } else {
        geometric_mean = -pow(-product, 1.0 / n);
    }
    
    if (isnan(geometric_mean) || isinf(geometric_mean)) {
        fprintf(stderr, "Error: Cannot compute geometric mean\n");
        return 1;
    }
    
    printf("Results for %d terms:\n", n);
    printf("Sum: %.6f\n", sum);
    printf("Product: %.6f\n", product);
    printf("Harmonic mean: %.6f\n", harmonic_mean);
    printf("Geometric mean: %.6f\n", geometric_mean);
    
    printf("Sequence verification (first 5 terms):\n");
    for (int m = 0; m < 5 && m < n; m++) {
        printf("Term %d: %.6f\n", m + 1, sequence[m]);
    }
    
    return 0;
}