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
    
    double mean = sum / n;
    double geometric_mean = pow(fabs(product), 1.0 / n);
    
    double variance = 0.0;
    for (int i = 0; i < n; i++) {
        double diff = sequence[i] - mean;
        variance += diff * diff;
    }
    variance /= n;
    
    double min_val = sequence[0];
    double max_val = sequence[0];
    for (int i = 1; i < n; i++) {
        if (sequence[i] < min_val) {
            min_val = sequence[i];
        }
        if (sequence[i] > max_val) {
            max_val = sequence[i];
        }
    }
    
    printf("Computed %d terms\n", n);
    printf("Arithmetic mean: %.6f\n", mean);
    printf("Geometric mean: %.6f\n", geometric_mean);
    printf("Variance: %.6f\n", variance);
    printf("Minimum: %.6f\n", min_val);
    printf("Maximum: %.6f\n", max_val);
    printf("Sum: %.6f\n", sum);
    printf("Product: %.6f\n", product);
    
    return 0;
}