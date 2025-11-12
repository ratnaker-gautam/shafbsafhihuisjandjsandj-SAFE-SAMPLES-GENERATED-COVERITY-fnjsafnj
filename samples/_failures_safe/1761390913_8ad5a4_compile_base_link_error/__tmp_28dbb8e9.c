//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define ARRAY_SIZE 100

int main() {
    int n;
    printf("Enter number of iterations (1-%d): ", MAX_ITERATIONS);
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (n < 1 || n > MAX_ITERATIONS) {
        fprintf(stderr, "Error: Iterations out of range\n");
        return 1;
    }
    
    double results[ARRAY_SIZE];
    if (n > ARRAY_SIZE) {
        fprintf(stderr, "Error: Too many iterations\n");
        return 1;
    }
    
    printf("Computing square roots and their squares...\n");
    
    int i = 0;
    while (i < n) {
        double value = (double)(i + 1);
        double sqrt_val = sqrt(value);
        results[i] = sqrt_val * sqrt_val;
        
        if (fabs(results[i] - value) > 1e-10) {
            fprintf(stderr, "Error: Numerical precision issue\n");
            return 1;
        }
        i++;
    }
    
    double sum = 0.0;
    for (int j = 0; j < n; j++) {
        sum += results[j];
    }
    
    double expected_sum = 0.0;
    int k = 1;
    do {
        expected_sum += k;
        k++;
    } while (k <= n);
    
    printf("Sum of computed values: %.6f\n", sum);
    printf("Expected sum (1+2+...+%d): %.6f\n", n, expected_sum);
    
    if (fabs(sum - expected_sum) > 1e-10) {
        fprintf(stderr, "Error: Sum mismatch\n");
        return 1;
    }
    
    printf("Verification passed. Computing geometric progression...\n");
    
    double progression[ARRAY_SIZE];
    if (n > ARRAY_SIZE) {
        fprintf(stderr, "Error: Array bounds exceeded\n");
        return 1;
    }
    
    progression[0] = 1.0;
    for (int idx = 1; idx < n; idx++) {
        progression[idx] = progression[idx-1] * 2.0;
        if (progression[idx] > 1e308) {
            fprintf(stderr, "Error: Numerical overflow\n");
            return 1;
        }
    }
    
    double geo_sum = 0.0;
    int m = 0;
    while (m < n) {
        geo_sum += progression[m];
        m++;
    }
    
    double expected_geo = pow(2.0, n) - 1.0;
    printf("Geometric progression sum: %.6f\n", geo_sum);
    printf("Expected geometric sum: %.6f\n", expected_geo);
    
    if (fabs(geo_sum - expected_geo) > 1e-6) {
        fprintf(stderr, "Warning: Geometric sum has small precision difference\n");
    }
    
    printf("All computations completed successfully.\n");
    return 0;
}