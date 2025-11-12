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
    
    printf("Computing sequence using different loop styles...\n");
    
    int i = 0;
    while (i < n) {
        double term = sin((i + 1) * 0.1);
        if (isnan(term) || isinf(term)) {
            fprintf(stderr, "Error: Numerical overflow in computation\n");
            return 1;
        }
        sequence[i] = term;
        i++;
    }
    
    for (int j = 0; j < n; j++) {
        sum += sequence[j];
    }
    
    int k = 0;
    do {
        if (fabs(sequence[k]) < 1e-10) {
            fprintf(stderr, "Error: Division by near-zero value\n");
            return 1;
        }
        product *= sequence[k];
        if (isnan(product) || isinf(product)) {
            fprintf(stderr, "Error: Numerical overflow in product\n");
            return 1;
        }
        k++;
    } while (k < n);
    
    double mean = sum / n;
    if (isnan(mean) || isinf(mean)) {
        fprintf(stderr, "Error: Invalid mean calculation\n");
        return 1;
    }
    
    double variance = 0.0;
    for (int m = 0; m < n; m++) {
        double diff = sequence[m] - mean;
        variance += diff * diff;
        if (isnan(variance) || isinf(variance)) {
            fprintf(stderr, "Error: Numerical overflow in variance\n");
            return 1;
        }
    }
    variance /= n;
    
    printf("Results:\n");
    printf("Sum: %.6f\n", sum);
    printf("Product: %.6f\n", product);
    printf("Mean: %.6f\n", mean);
    printf("Variance: %.6f\n", variance);
    
    double running_sum = 0.0;
    printf("Running sums: ");
    for (int p = 0; p < n; p++) {
        running_sum += sequence[p];
        printf("%.3f", running_sum);
        if (p < n - 1) {
            printf(", ");
        }
        if (isnan(running_sum) || isinf(running_sum)) {
            fprintf(stderr, "\nError: Numerical overflow in running sum\n");
            return 1;
        }
    }
    printf("\n");
    
    return 0;
}