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
    
    printf("Computing sequence using different loop styles...\n");
    
    int i = 0;
    while (i < n) {
        sequence[i] = sin((double)(i + 1) * 0.1);
        i++;
    }
    
    for (int j = 0; j < n; j++) {
        sum += sequence[j];
    }
    
    double product = 1.0;
    int k = 0;
    do {
        if (k < n) {
            product *= (1.0 + sequence[k] * 0.1);
        }
        k++;
    } while (k < n);
    
    double running_sum = 0.0;
    double* ptr = sequence;
    for (int m = 0; m < n; m++) {
        running_sum += *ptr;
        ptr++;
    }
    
    double harmonic_mean = 0.0;
    for (int p = 0; p < n; p++) {
        if (fabs(sequence[p]) > 1e-10) {
            harmonic_mean += 1.0 / sequence[p];
        }
    }
    if (fabs(harmonic_mean) > 1e-10) {
        harmonic_mean = (double)n / harmonic_mean;
    }
    
    printf("Results for %d terms:\n", n);
    printf("Sum: %.6f\n", sum);
    printf("Running sum: %.6f\n", running_sum);
    printf("Product: %.6f\n", product);
    printf("Harmonic mean: %.6f\n", harmonic_mean);
    
    double min_val = sequence[0];
    double max_val = sequence[0];
    for (int q = 1; q < n; q++) {
        if (sequence[q] < min_val) min_val = sequence[q];
        if (sequence[q] > max_val) max_val = sequence[q];
    }
    
    printf("Range: [%.6f, %.6f]\n", min_val, max_val);
    
    return 0;
}