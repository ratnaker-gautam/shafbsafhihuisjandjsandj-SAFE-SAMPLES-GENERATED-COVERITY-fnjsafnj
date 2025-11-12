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
        double term = sin((i + 1) * 0.1) + cos(i * 0.05);
        if (isnan(term) || isinf(term)) {
            fprintf(stderr, "Error: Numerical overflow in computation\n");
            return 1;
        }
        sequence[i] = term;
        i++;
    }
    
    for (i = 0; i < n; i++) {
        sum += sequence[i];
    }
    
    i = 0;
    do {
        if (fabs(sequence[i]) < 1e-10) {
            fprintf(stderr, "Error: Division by zero in product calculation\n");
            return 1;
        }
        product *= sequence[i];
        if (isinf(product) || isnan(product)) {
            fprintf(stderr, "Error: Numerical overflow in product\n");
            return 1;
        }
        i++;
    } while (i < n);
    
    double harmonic_mean = 0.0;
    for (int j = 0; j < n; j++) {
        if (fabs(sequence[j]) < 1e-10) {
            fprintf(stderr, "Error: Cannot compute harmonic mean with zero values\n");
            return 1;
        }
        harmonic_mean += 1.0 / sequence[j];
    }
    harmonic_mean = n / harmonic_mean;
    
    double geometric_mean = pow(fabs(product), 1.0 / n);
    if (product < 0) {
        geometric_mean = -geometric_mean;
    }
    
    printf("Results for %d terms:\n", n);
    printf("Sum: %.6f\n", sum);
    printf("Product: %.6f\n", product);
    printf("Arithmetic mean: %.6f\n", sum / n);
    printf("Geometric mean: %.6f\n", geometric_mean);
    printf("Harmonic mean: %.6f\n", harmonic_mean);
    
    int count = 0;
    for (int k = 0; k < n; k++) {
        if (sequence[k] > 0) {
            count++;
        }
    }
    printf("Positive terms: %d\n", count);
    
    return 0;
}