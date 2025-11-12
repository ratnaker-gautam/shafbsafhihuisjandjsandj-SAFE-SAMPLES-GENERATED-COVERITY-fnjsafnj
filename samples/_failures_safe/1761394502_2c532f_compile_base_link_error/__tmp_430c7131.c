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
        if (isnan(term) || isinf(term)) {
            fprintf(stderr, "Error: Numerical overflow in computation\n");
            return 1;
        }
        sequence[i] = term;
        i++;
    }
    
    for (int j = 0; j < n; j++) {
        sum += sequence[j];
        if (isinf(sum) || isnan(sum)) {
            fprintf(stderr, "Error: Sum overflow\n");
            return 1;
        }
    }
    
    int k = 0;
    do {
        if (k >= n) break;
        product *= sequence[k];
        if (isinf(product) || isnan(product) || fabs(product) > 1e100) {
            fprintf(stderr, "Error: Product overflow\n");
            return 1;
        }
        k++;
    } while (k < n);
    
    double harmonic_mean = 0.0;
    int m = 0;
    while (m < n) {
        if (fabs(sequence[m]) < 1e-10) {
            fprintf(stderr, "Error: Division by zero in harmonic mean\n");
            return 1;
        }
        harmonic_mean += 1.0 / sequence[m];
        if (isinf(harmonic_mean) || isnan(harmonic_mean)) {
            fprintf(stderr, "Error: Harmonic mean overflow\n");
            return 1;
        }
        m++;
    }
    harmonic_mean = n / harmonic_mean;
    
    printf("Results for %d terms:\n", n);
    printf("Sum: %.6f\n", sum);
    printf("Product: %.6f\n", product);
    printf("Arithmetic mean: %.6f\n", sum / n);
    printf("Harmonic mean: %.6f\n", harmonic_mean);
    
    double min_val = sequence[0];
    double max_val = sequence[0];
    for (int idx = 1; idx < n; idx++) {
        if (sequence[idx] < min_val) min_val = sequence[idx];
        if (sequence[idx] > max_val) max_val = sequence[idx];
    }
    printf("Minimum: %.6f\n", min_val);
    printf("Maximum: %.6f\n", max_val);
    
    return 0;
}