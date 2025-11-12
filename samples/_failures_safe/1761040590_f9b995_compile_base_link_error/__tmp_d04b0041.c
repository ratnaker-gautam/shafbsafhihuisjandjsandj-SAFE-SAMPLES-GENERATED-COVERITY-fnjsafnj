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
        if (fabs(sequence[j]) > 1e-10) {
            product *= sequence[j];
        }
        j++;
    } while (j < n);
    
    double mean = sum / n;
    double geometric_mean;
    if (product >= 0) {
        geometric_mean = pow(product, 1.0 / n);
    } else {
        geometric_mean = -pow(-product, 1.0 / n);
    }
    
    double variance = 0.0;
    for (int k = 0; k < n; k++) {
        double diff = sequence[k] - mean;
        variance += diff * diff;
    }
    variance /= n;
    
    double min_val = sequence[0];
    double max_val = sequence[0];
    int pos = 0;
    while (pos < n) {
        if (sequence[pos] < min_val) min_val = sequence[pos];
        if (sequence[pos] > max_val) max_val = sequence[pos];
        pos++;
    }
    
    printf("Sequence statistics for %d terms:\n", n);
    printf("Arithmetic mean: %.6f\n", mean);
    printf("Geometric mean: %.6f\n", geometric_mean);
    printf("Variance: %.6f\n", variance);
    printf("Minimum: %.6f\n", min_val);
    printf("Maximum: %.6f\n", max_val);
    printf("Sum: %.6f\n", sum);
    printf("Product: %.6f\n", product);
    
    return 0;
}