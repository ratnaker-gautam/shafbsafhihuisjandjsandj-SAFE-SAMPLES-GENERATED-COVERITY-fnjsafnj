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
    
    printf("Computing sequence terms and statistics...\n");
    
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
        if (fabs(product) > 1e300 && fabs(sequence[j]) > 1.0) {
            fprintf(stderr, "Error: Product would overflow\n");
            return 1;
        }
        product *= sequence[j];
        if (isnan(product) || isinf(product)) {
            fprintf(stderr, "Error: Numerical overflow in product\n");
            return 1;
        }
    }
    
    double min_val = sequence[0];
    double max_val = sequence[0];
    int k = 1;
    do {
        if (sequence[k] < min_val) {
            min_val = sequence[k];
        }
        if (sequence[k] > max_val) {
            max_val = sequence[k];
        }
        k++;
    } while (k < n);
    
    double mean = sum / n;
    double variance = 0.0;
    
    for (int m = 0; m < n; m++) {
        double diff = sequence[m] - mean;
        variance += diff * diff;
    }
    variance /= n;
    
    printf("Sequence statistics for %d terms:\n", n);
    printf("Sum: %.6f\n", sum);
    printf("Product: %.6f\n", product);
    printf("Mean: %.6f\n", mean);
    printf("Variance: %.6f\n", variance);
    printf("Min: %.6f\n", min_val);
    printf("Max: %.6f\n", max_val);
    
    printf("First 5 terms: ");
    int count = 0;
    for (int p = 0; p < n && count < 5; p++) {
        printf("%.3f ", sequence[p]);
        count++;
    }
    printf("\n");
    
    return 0;
}