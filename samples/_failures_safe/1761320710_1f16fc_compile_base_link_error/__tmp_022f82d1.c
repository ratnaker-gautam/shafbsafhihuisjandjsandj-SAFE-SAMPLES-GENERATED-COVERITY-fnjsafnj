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
        fprintf(stderr, "Error: Number out of range\n");
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
        double square_val = sqrt_val * sqrt_val;
        
        if (square_val < 0 || isnan(square_val) || isinf(square_val)) {
            fprintf(stderr, "Error: Numerical overflow detected\n");
            return 1;
        }
        
        results[i] = square_val - value;
        i++;
    }

    printf("Results (difference between squared sqrt and original):\n");
    for (int j = 0; j < n; j++) {
        printf("Iteration %d: %.10f\n", j + 1, results[j]);
    }

    double sum = 0.0;
    int k = 0;
    do {
        if (k >= n) break;
        sum += results[k];
        k++;
    } while (k < n);

    printf("Sum of differences: %.10f\n", sum);

    double max_diff = 0.0;
    for (int m = 0; m < n; m++) {
        double abs_diff = fabs(results[m]);
        if (abs_diff > max_diff) {
            max_diff = abs_diff;
        }
    }

    printf("Maximum absolute difference: %.10f\n", max_diff);

    return 0;
}