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
    
    int i = 0;
    while (i < n) {
        sequence[i] = sin((double)(i + 1) * 0.1);
        i++;
    }
    
    for (int j = 0; j < n; j++) {
        sum += sequence[j];
    }
    
    int k = 0;
    do {
        product *= sequence[k];
        k++;
    } while (k < n);
    
    double harmonic_mean = 0.0;
    for (int m = 0; m < n; m++) {
        if (sequence[m] == 0.0) {
            fprintf(stderr, "Error: Division by zero\n");
            return 1;
        }
        harmonic_mean += 1.0 / sequence[m];
    }
    harmonic_mean = (double)n / harmonic_mean;
    
    double geometric_mean = pow(fabs(product), 1.0 / (double)n);
    
    printf("Sequence statistics for %d terms:\n", n);
    printf("Sum: %.6f\n", sum);
    printf("Product: %.6f\n", product);
    printf("Arithmetic mean: %.6f\n", sum / (double)n);
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
    printf("Maximum: %.6f\n", max_val);
    printf("Minimum: %.6f\n", min_val);
    
    return 0;
}