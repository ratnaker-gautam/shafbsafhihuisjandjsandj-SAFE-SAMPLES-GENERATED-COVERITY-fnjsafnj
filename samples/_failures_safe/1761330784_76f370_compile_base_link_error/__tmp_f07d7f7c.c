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
        if (k < n) {
            product *= sequence[k];
        }
        k++;
    } while (k < n);
    
    double harmonic_mean = 0.0;
    for (int i = 0; i < n; i++) {
        if (sequence[i] != 0.0) {
            harmonic_mean += 1.0 / sequence[i];
        }
    }
    if (harmonic_mean != 0.0) {
        harmonic_mean = n / harmonic_mean;
    }
    
    double max_val = sequence[0];
    double min_val = sequence[0];
    for (int i = 1; i < n; i++) {
        if (sequence[i] > max_val) {
            max_val = sequence[i];
        }
        if (sequence[i] < min_val) {
            min_val = sequence[i];
        }
    }
    
    printf("Sequence statistics for %d terms:\n", n);
    printf("Sum: %.6f\n", sum);
    printf("Product: %.6f\n", product);
    printf("Harmonic mean: %.6f\n", harmonic_mean);
    printf("Maximum: %.6f\n", max_val);
    printf("Minimum: %.6f\n", min_val);
    
    double running_sum = 0.0;
    printf("Running averages:\n");
    for (int i = 0; i < n; i++) {
        running_sum += sequence[i];
        double avg = running_sum / (i + 1);
        printf("Term %d: %.6f (avg: %.6f)\n", i + 1, sequence[i], avg);
    }
    
    return 0;
}