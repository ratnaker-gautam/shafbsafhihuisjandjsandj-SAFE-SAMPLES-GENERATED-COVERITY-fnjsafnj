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
        double term = sin((double)(i + 1) * 0.1);
        if (isnan(term) || isinf(term)) {
            fprintf(stderr, "Error: Numerical overflow in computation\n");
            return 1;
        }
        sequence[i] = term;
        i++;
    }
    
    for (i = 0; i < n; i++) {
        sum += sequence[i];
        if (isinf(sum) || isnan(sum)) {
            fprintf(stderr, "Error: Sum overflow\n");
            return 1;
        }
    }
    
    i = 0;
    do {
        product *= sequence[i];
        if (isinf(product) || isnan(product) || fabs(product) > 1e300) {
            fprintf(stderr, "Error: Product overflow\n");
            return 1;
        }
        i++;
    } while (i < n);
    
    double geometric_mean;
    if (product >= 0) {
        geometric_mean = pow(product, 1.0 / n);
    } else {
        geometric_mean = -pow(-product, 1.0 / n);
    }
    
    if (isnan(geometric_mean) || isinf(geometric_mean)) {
        fprintf(stderr, "Error: Geometric mean computation failed\n");
        return 1;
    }
    
    double harmonic_mean = 0.0;
    for (i = 0; i < n; i++) {
        if (fabs(sequence[i]) < 1e-15) {
            fprintf(stderr, "Error: Cannot compute harmonic mean with zero terms\n");
            return 1;
        }
        harmonic_mean += 1.0 / sequence[i];
    }
    harmonic_mean = n / harmonic_mean;
    
    printf("Results for %d terms:\n", n);
    printf("Arithmetic mean: %.6f\n", sum / n);
    printf("Geometric mean: %.6f\n", geometric_mean);
    printf("Harmonic mean: %.6f\n", harmonic_mean);
    
    double min_val = sequence[0];
    double max_val = sequence[0];
    for (i = 1; i < n; i++) {
        if (sequence[i] < min_val) min_val = sequence[i];
        if (sequence[i] > max_val) max_val = sequence[i];
    }
    
    printf("Minimum value: %.6f\n", min_val);
    printf("Maximum value: %.6f\n", max_val);
    
    return 0;
}