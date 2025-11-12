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
    
    for (int i = 0; i < n; i++) {
        double term = sin((i + 1) * 0.1) + cos((i + 1) * 0.05);
        sequence[i] = term;
        sum += term;
        if (i > 0 && product > 0 && term > 0) {
            if (product > 1e308 / term) {
                fprintf(stderr, "Error: Product would overflow\n");
                return 1;
            }
            product *= term;
        }
    }
    
    double mean = sum / n;
    double variance = 0.0;
    
    int j = 0;
    while (j < n) {
        double diff = sequence[j] - mean;
        variance += diff * diff;
        j++;
    }
    variance /= n;
    
    double geometric_mean;
    if (n > 0 && product > 0) {
        geometric_mean = pow(product, 1.0 / n);
    } else {
        geometric_mean = 0.0;
    }
    
    int k = 0;
    double min_val = sequence[0];
    double max_val = sequence[0];
    do {
        if (sequence[k] < min_val) min_val = sequence[k];
        if (sequence[k] > max_val) max_val = sequence[k];
        k++;
    } while (k < n);
    
    printf("Sequence statistics for %d terms:\n", n);
    printf("Arithmetic mean: %.6f\n", mean);
    printf("Geometric mean: %.6f\n", geometric_mean);
    printf("Variance: %.6f\n", variance);
    printf("Minimum: %.6f\n", min_val);
    printf("Maximum: %.6f\n", max_val);
    printf("Sum: %.6f\n", sum);
    
    printf("\nFirst 10 terms: ");
    for (int i = 0; i < (n < 10 ? n : 10); i++) {
        printf("%.3f ", sequence[i]);
    }
    printf("\n");
    
    return 0;
}