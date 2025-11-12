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
    
    for (int i = 0; i < n; i++) {
        sequence[i] = sin((i + 1) * 0.1) + cos((i + 1) * 0.05);
    }
    
    int j = 0;
    while (j < n) {
        sum += sequence[j];
        j++;
    }
    
    int k = 0;
    do {
        if (fabs(sequence[k]) > 1e-10) {
            product *= sequence[k];
        }
        k++;
    } while (k < n);
    
    double harmonic_mean = 0.0;
    for (int i = 0; i < n; i++) {
        if (fabs(sequence[i]) > 1e-10) {
            harmonic_mean += 1.0 / sequence[i];
        }
    }
    if (fabs(harmonic_mean) > 1e-10) {
        harmonic_mean = n / harmonic_mean;
    } else {
        harmonic_mean = 0.0;
    }
    
    double geometric_mean;
    if (product >= 0) {
        geometric_mean = pow(product, 1.0 / n);
    } else {
        geometric_mean = -pow(-product, 1.0 / n);
    }
    
    double arithmetic_mean = sum / n;
    
    printf("Sequence statistics for %d terms:\n", n);
    printf("Arithmetic mean: %.6f\n", arithmetic_mean);
    printf("Geometric mean: %.6f\n", geometric_mean);
    printf("Harmonic mean: %.6f\n", harmonic_mean);
    
    double max_val = sequence[0];
    double min_val = sequence[0];
    for (int i = 1; i < n; i++) {
        if (sequence[i] > max_val) max_val = sequence[i];
        if (sequence[i] < min_val) min_val = sequence[i];
    }
    
    printf("Maximum value: %.6f\n", max_val);
    printf("Minimum value: %.6f\n", min_val);
    printf("Range: %.6f\n", max_val - min_val);
    
    return 0;
}