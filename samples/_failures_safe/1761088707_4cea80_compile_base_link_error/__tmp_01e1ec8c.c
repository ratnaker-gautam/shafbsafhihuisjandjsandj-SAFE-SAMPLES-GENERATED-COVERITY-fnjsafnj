//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
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
        fprintf(stderr, "Error: Number out of range\n");
        return 1;
    }
    
    double sequence[ARRAY_SIZE];
    double sum = 0.0;
    double product = 1.0;
    
    int i = 0;
    while (i < n) {
        double term = sin((i + 1) * 0.1);
        if (isnan(term) || isinf(term)) {
            fprintf(stderr, "Error: Numerical computation failed\n");
            return 1;
        }
        sequence[i] = term;
        i++;
    }
    
    for (int j = 0; j < n; j++) {
        sum += sequence[j];
    }
    
    int k = 0;
    do {
        product *= sequence[k];
        if (isnan(product) || isinf(product)) {
            fprintf(stderr, "Error: Numerical overflow in product\n");
            return 1;
        }
        k++;
    } while (k < n);
    
    double harmonic_mean = 0.0;
    for (int m = 0; m < n; m++) {
        if (sequence[m] == 0.0) {
            fprintf(stderr, "Error: Cannot compute harmonic mean with zero values\n");
            return 1;
        }
        harmonic_mean += 1.0 / sequence[m];
    }
    harmonic_mean = n / harmonic_mean;
    
    double geometric_mean = pow(fabs(product), 1.0 / n);
    
    printf("Sequence statistics for %d terms:\n", n);
    printf("Sum: %.6f\n", sum);
    printf("Product: %.6f\n", product);
    printf("Arithmetic mean: %.6f\n", sum / n);
    printf("Geometric mean: %.6f\n", geometric_mean);
    printf("Harmonic mean: %.6f\n", harmonic_mean);
    
    double max_val = sequence[0];
    double min_val = sequence[0];
    int idx = 0;
    while (idx < n) {
        if (sequence[idx] > max_val) {
            max_val = sequence[idx];
        }
        if (sequence[idx] < min_val) {
            min_val = sequence[idx];
        }
        idx++;
    }
    
    printf("Maximum value: %.6f\n", max_val);
    printf("Minimum value: %.6f\n", min_val);
    
    return 0;
}