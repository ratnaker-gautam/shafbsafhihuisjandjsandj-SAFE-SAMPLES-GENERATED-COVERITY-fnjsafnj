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
    
    printf("Computing sequence using different loop variants:\n");
    
    int i = 0;
    while (i < n) {
        sequence[i] = sqrt(i + 1.0);
        if (isnan(sequence[i]) || isinf(sequence[i])) {
            fprintf(stderr, "Error: Numerical overflow in computation\n");
            return 1;
        }
        i++;
    }
    
    for (int j = 0; j < n; j++) {
        double term = sequence[j] / (j + 1.0);
        if (isnan(term) || isinf(term)) {
            fprintf(stderr, "Error: Numerical overflow in division\n");
            return 1;
        }
        sum += term;
    }
    
    double product = 1.0;
    int k = 0;
    do {
        if (k >= n) break;
        double factor = 1.0 + (sequence[k] / 10.0);
        if (isnan(factor) || isinf(factor) || factor == 0.0) {
            fprintf(stderr, "Error: Invalid factor in product\n");
            return 1;
        }
        product *= factor;
        k++;
    } while (k < n);
    
    double harmonic_mean = 0.0;
    for (int m = 0; m < n; m++) {
        if (sequence[m] == 0.0) {
            fprintf(stderr, "Error: Division by zero in harmonic mean\n");
            return 1;
        }
        harmonic_mean += 1.0 / sequence[m];
    }
    harmonic_mean = n / harmonic_mean;
    
    printf("Results for %d terms:\n", n);
    printf("Sum of sequence terms: %.6f\n", sum);
    printf("Product of factors: %.6f\n", product);
    printf("Harmonic mean: %.6f\n", harmonic_mean);
    
    double running_avg = 0.0;
    printf("Running averages: ");
    for (int idx = 0; idx < n; idx++) {
        running_avg = (running_avg * idx + sequence[idx]) / (idx + 1.0);
        if (isnan(running_avg) || isinf(running_avg)) {
            fprintf(stderr, "\nError: Numerical error in running average\n");
            return 1;
        }
        printf("%.3f ", running_avg);
        if ((idx + 1) % 5 == 0) printf("\n");
    }
    printf("\n");
    
    return 0;
}