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
    
    double variance_sum = 0.0;
    for (int k = 0; k < n; k++) {
        double diff = sequence[k] - mean;
        variance_sum += diff * diff;
    }
    double variance = variance_sum / n;
    double std_dev = sqrt(variance);
    
    printf("Sequence statistics for %d terms:\n", n);
    printf("Arithmetic mean: %.6f\n", mean);
    printf("Geometric mean: %.6f\n", geometric_mean);
    printf("Standard deviation: %.6f\n", std_dev);
    
    printf("\nFirst 10 sequence values:\n");
    for (int m = 0; m < (n < 10 ? n : 10); m++) {
        printf("Term %d: %.6f\n", m + 1, sequence[m]);
    }
    
    double running_sum = 0.0;
    printf("\nRunning sums:\n");
    for (int p = 0; p < n; p++) {
        running_sum += sequence[p];
        printf("After term %d: %.6f\n", p + 1, running_sum);
    }
    
    return 0;
}