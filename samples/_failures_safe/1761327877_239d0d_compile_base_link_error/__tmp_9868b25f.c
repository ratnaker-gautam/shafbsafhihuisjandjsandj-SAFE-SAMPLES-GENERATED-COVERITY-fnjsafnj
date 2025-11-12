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
    
    int i = 0;
    while (i < n) {
        sum += sequence[i];
        i++;
    }
    
    int j = 0;
    do {
        if (j < n) {
            product *= sequence[j];
        }
        j++;
    } while (j < n);
    
    double harmonic_mean = 0.0;
    for (int k = 0; k < n; k++) {
        if (sequence[k] != 0.0) {
            harmonic_mean += 1.0 / sequence[k];
        }
    }
    if (harmonic_mean != 0.0) {
        harmonic_mean = n / harmonic_mean;
    }
    
    double geometric_mean;
    if (product >= 0.0) {
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
    for (int idx = 1; idx < n; idx++) {
        if (sequence[idx] > max_val) {
            max_val = sequence[idx];
        }
        if (sequence[idx] < min_val) {
            min_val = sequence[idx];
        }
    }
    
    printf("Maximum value: %.6f\n", max_val);
    printf("Minimum value: %.6f\n", min_val);
    printf("Range: %.6f\n", max_val - min_val);
    
    double variance = 0.0;
    for (int m = 0; m < n; m++) {
        double diff = sequence[m] - arithmetic_mean;
        variance += diff * diff;
    }
    variance /= n;
    double std_dev = sqrt(variance);
    
    printf("Variance: %.6f\n", variance);
    printf("Standard deviation: %.6f\n", std_dev);
    
    return 0;
}