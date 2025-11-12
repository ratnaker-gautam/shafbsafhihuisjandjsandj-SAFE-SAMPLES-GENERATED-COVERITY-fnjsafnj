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
        fprintf(stderr, "Error: Number out of range\n");
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
    
    double product = 1.0;
    int k = 0;
    do {
        if (k < n) {
            product *= sequence[k];
        }
        k++;
    } while (k < n);
    
    double harmonic_mean = 0.0;
    for (int m = 0; m < n; m++) {
        if (sequence[m] != 0.0) {
            harmonic_mean += 1.0 / sequence[m];
        }
    }
    if (harmonic_mean != 0.0) {
        harmonic_mean = (double)n / harmonic_mean;
    }
    
    double geometric_mean = pow(product, 1.0 / (double)n);
    double arithmetic_mean = sum / (double)n;
    
    printf("Results for %d terms:\n", n);
    printf("Arithmetic mean: %.6f\n", arithmetic_mean);
    printf("Geometric mean: %.6f\n", geometric_mean);
    printf("Harmonic mean: %.6f\n", harmonic_mean);
    
    double max_val = sequence[0];
    double min_val = sequence[0];
    for (int p = 1; p < n; p++) {
        if (sequence[p] > max_val) {
            max_val = sequence[p];
        }
        if (sequence[p] < min_val) {
            min_val = sequence[p];
        }
    }
    
    printf("Maximum value: %.6f\n", max_val);
    printf("Minimum value: %.6f\n", min_val);
    
    double variance = 0.0;
    for (int q = 0; q < n; q++) {
        double diff = sequence[q] - arithmetic_mean;
        variance += diff * diff;
    }
    variance /= (double)n;
    double std_dev = sqrt(variance);
    
    printf("Standard deviation: %.6f\n", std_dev);
    
    return 0;
}