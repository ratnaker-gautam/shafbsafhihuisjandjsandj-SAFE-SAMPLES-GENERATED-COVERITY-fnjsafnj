//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main() {
    int n;
    double sum = 0.0;
    double product = 1.0;
    double mean, geometric_mean;
    int valid_inputs = 0;
    
    printf("Enter number of values (2-100): ");
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (n < 2 || n > 100) {
        fprintf(stderr, "Error: Number must be between 2 and 100\n");
        return 1;
    }
    
    double *values = malloc(n * sizeof(double));
    if (values == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return 1;
    }
    
    printf("Enter %d positive numbers:\n", n);
    for (int i = 0; i < n; i++) {
        printf("Value %d: ", i + 1);
        if (scanf("%lf", &values[i]) != 1) {
            fprintf(stderr, "Error: Invalid input\n");
            free(values);
            return 1;
        }
        
        if (values[i] <= 0.0) {
            fprintf(stderr, "Error: Values must be positive\n");
            free(values);
            return 1;
        }
        
        if (values[i] > 1e308) {
            fprintf(stderr, "Error: Value too large\n");
            free(values);
            return 1;
        }
        
        sum += values[i];
        product *= values[i];
        valid_inputs++;
    }
    
    mean = sum / n;
    
    if (product <= 0.0) {
        fprintf(stderr, "Error: Product calculation failed\n");
        free(values);
        return 1;
    }
    
    geometric_mean = pow(product, 1.0 / n);
    
    if (!isfinite(geometric_mean)) {
        fprintf(stderr, "Error: Geometric mean calculation failed\n");
        free(values);
        return 1;
    }
    
    printf("\nResults:\n");
    printf("Arithmetic mean: %.6f\n", mean);
    printf("Geometric mean: %.6f\n", geometric_mean);
    printf("Ratio (arithmetic/geometric): %.6f\n", mean / geometric_mean);
    
    double variance = 0.0;
    for (int i = 0; i < n; i++) {
        double diff = values[i] - mean;
        variance += diff * diff;
    }
    variance /= n;
    
    double std_dev = sqrt(variance);
    printf("Standard deviation: %.6f\n", std_dev);
    
    free(values);
    return 0;
}