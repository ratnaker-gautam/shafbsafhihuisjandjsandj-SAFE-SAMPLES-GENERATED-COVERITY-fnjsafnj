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
    
    printf("Computing sequence terms...\n");
    
    int i = 0;
    while (i < n) {
        double term = sin((i + 1) * 0.1) + cos(i * 0.05);
        if (!isfinite(term)) {
            fprintf(stderr, "Error: Numerical overflow in computation\n");
            return 1;
        }
        sequence[i] = term;
        i++;
    }
    
    for (int j = 0; j < n; j++) {
        sum += sequence[j];
        if (!isfinite(sum)) {
            fprintf(stderr, "Error: Sum overflow\n");
            return 1;
        }
    }
    
    int k = 0;
    do {
        product *= sequence[k];
        if (!isfinite(product)) {
            fprintf(stderr, "Error: Product overflow\n");
            return 1;
        }
        k++;
    } while (k < n && k < ARRAY_SIZE);
    
    double mean = sum / n;
    if (!isfinite(mean)) {
        fprintf(stderr, "Error: Mean computation failed\n");
        return 1;
    }
    
    double variance_sum = 0.0;
    for (int m = 0; m < n; m++) {
        double diff = sequence[m] - mean;
        variance_sum += diff * diff;
        if (!isfinite(variance_sum)) {
            fprintf(stderr, "Error: Variance computation overflow\n");
            return 1;
        }
    }
    
    double variance = variance_sum / n;
    if (!isfinite(variance)) {
        fprintf(stderr, "Error: Variance computation failed\n");
        return 1;
    }
    
    printf("Results for %d terms:\n", n);
    printf("Sum: %.6f\n", sum);
    printf("Product: %.6f\n", product);
    printf("Mean: %.6f\n", mean);
    printf("Variance: %.6f\n", variance);
    
    printf("First 5 terms: ");
    int count = (n < 5) ? n : 5;
    for (int p = 0; p < count; p++) {
        printf("%.4f ", sequence[p]);
    }
    printf("\n");
    
    return 0;
}