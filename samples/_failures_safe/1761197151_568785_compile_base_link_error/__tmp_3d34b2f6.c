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
        if (term != term) {
            fprintf(stderr, "Error: Numerical computation failed\n");
            return 1;
        }
        sequence[i] = term;
        i++;
    }
    
    for (int j = 0; j < n; j++) {
        sum += sequence[j];
        if (j > 0) {
            if (fabs(sequence[j]) < 1e-10) {
                fprintf(stderr, "Error: Division by zero in product\n");
                return 1;
            }
            product *= sequence[j];
        } else {
            product = sequence[j];
        }
    }
    
    double mean = sum / n;
    double geometric_mean;
    if (product >= 0) {
        geometric_mean = pow(product, 1.0 / n);
    } else {
        geometric_mean = -pow(-product, 1.0 / n);
    }
    
    if (geometric_mean != geometric_mean) {
        fprintf(stderr, "Error: Geometric mean computation failed\n");
        return 1;
    }
    
    double variance_sum = 0.0;
    int k = 0;
    do {
        double diff = sequence[k] - mean;
        variance_sum += diff * diff;
        k++;
    } while (k < n);
    
    double variance = variance_sum / n;
    double std_dev = sqrt(variance);
    
    if (std_dev != std_dev) {
        fprintf(stderr, "Error: Standard deviation computation failed\n");
        return 1;
    }
    
    printf("Results for %d terms:\n", n);
    printf("Arithmetic mean: %.6f\n", mean);
    printf("Geometric mean: %.6f\n", geometric_mean);
    printf("Standard deviation: %.6f\n", std_dev);
    
    printf("First 10 terms: ");
    for (int m = 0; m < (n < 10 ? n : 10); m++) {
        printf("%.4f ", sequence[m]);
    }
    printf("\n");
    
    return 0;
}