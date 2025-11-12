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
    } while (k <= n);
    
    double running_sum = 0.0;
    double running_avg[ARRAY_SIZE];
    for (int m = 0; m < n; m++) {
        running_sum += sequence[m];
        running_avg[m] = running_sum / (m + 1);
    }
    
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
    
    printf("Results for %d terms:\n", n);
    printf("Sum: %.6f\n", sum);
    printf("Product: %.6f\n", product);
    printf("Average: %.6f\n", sum / n);
    printf("Maximum: %.6f\n", max_val);
    printf("Minimum: %.6f\n", min_val);
    printf("First 5 terms: ");
    for (int q = 0; q < (n < 5 ? n : 5); q++) {
        printf("%.3f ", sequence[q]);
    }
    printf("\n");
    
    double variance = 0.0;
    double mean = sum / n;
    for (int r = 0; r < n; r++) {
        double diff = sequence[r] - mean;
        variance += diff * diff;
    }
    variance /= n;
    
    printf("Variance: %.6f\n", variance);
    printf("Standard deviation: %.6f\n", sqrt(variance));
    
    return 0;
}