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
    }
    
    double mean = sum / n;
    double variance = 0.0;
    
    int k = 0;
    do {
        if (k < n) {
            double diff = sequence[k] - mean;
            variance += diff * diff;
        }
        k++;
    } while (k < n);
    
    variance /= n;
    double std_dev = sqrt(variance);
    
    double min_val = sequence[0];
    double max_val = sequence[0];
    
    for (int idx = 1; idx < n; idx++) {
        if (sequence[idx] < min_val) {
            min_val = sequence[idx];
        }
        if (sequence[idx] > max_val) {
            max_val = sequence[idx];
        }
    }
    
    printf("Results for %d terms:\n", n);
    printf("Sum: %.6f\n", sum);
    printf("Product: %.6f\n", product);
    printf("Mean: %.6f\n", mean);
    printf("Standard deviation: %.6f\n", std_dev);
    printf("Minimum: %.6f\n", min_val);
    printf("Maximum: %.6f\n", max_val);
    
    printf("First 5 terms: ");
    int count = 0;
    for (int m = 0; m < n && count < 5; m++) {
        printf("%.4f ", sequence[m]);
        count++;
    }
    printf("\n");
    
    return 0;
}