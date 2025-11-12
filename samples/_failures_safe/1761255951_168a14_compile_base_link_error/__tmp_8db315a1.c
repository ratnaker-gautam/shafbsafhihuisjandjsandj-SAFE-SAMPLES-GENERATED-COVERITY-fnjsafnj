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
    
    printf("Computing sequence using different loop variants:\n");
    
    int i = 0;
    while (i < n) {
        sequence[i] = sqrt((double)(i + 1));
        i++;
    }
    
    for (int j = 0; j < n; j++) {
        sum += sequence[j];
    }
    
    double harmonic_mean = 0.0;
    int k = 0;
    do {
        if (sequence[k] != 0.0) {
            harmonic_mean += 1.0 / sequence[k];
        }
        k++;
    } while (k < n);
    
    if (harmonic_mean != 0.0) {
        harmonic_mean = (double)n / harmonic_mean;
    }
    
    double geometric_mean = 1.0;
    for (int m = 0; m < n; m++) {
        geometric_mean *= sequence[m];
        if (geometric_mean > 1e308 || geometric_mean < 1e-308) {
            geometric_mean = 0.0;
            break;
        }
    }
    
    if (geometric_mean > 0.0 && n > 0) {
        geometric_mean = pow(geometric_mean, 1.0 / (double)n);
    }
    
    printf("Results for %d terms:\n", n);
    printf("Arithmetic mean: %.6f\n", sum / (double)n);
    printf("Harmonic mean: %.6f\n", harmonic_mean);
    printf("Geometric mean: %.6f\n", geometric_mean);
    
    double running_sum = 0.0;
    printf("Running sums: ");
    for (int p = 0; p < n; p++) {
        running_sum += sequence[p];
        printf("%.2f", running_sum);
        if (p < n - 1) {
            printf(", ");
        }
    }
    printf("\n");
    
    return 0;
}