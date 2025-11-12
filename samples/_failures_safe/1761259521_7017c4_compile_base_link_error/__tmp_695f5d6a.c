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

    printf("Computing square roots using different loop variants...\n");
    
    int i = 0;
    while (i < n) {
        double value = (double)(i + 1);
        results[i] = sqrt(value);
        i++;
    }

    printf("While loop results (first 10):\n");
    for (int j = 0; j < 10 && j < n; j++) {
        printf("sqrt(%d) = %.6f\n", j + 1, results[j]);
    }

    double sum = 0.0;
    int k = 0;
    do {
        if (k < n) {
            sum += results[k];
        }
        k++;
    } while (k < n);

    printf("Sum of all results: %.6f\n", sum);

    double product = 1.0;
    for (int m = 0; m < n; m++) {
        if (results[m] != 0.0) {
            if (product > 1e308 / results[m]) {
                fprintf(stderr, "Error: Potential overflow\n");
                return 1;
            }
            product *= results[m];
        }
    }

    printf("Product of all results: %.6e\n", product);

    double max_val = results[0];
    double min_val = results[0];
    for (int idx = 1; idx < n; idx++) {
        if (results[idx] > max_val) {
            max_val = results[idx];
        }
        if (results[idx] < min_val) {
            min_val = results[idx];
        }
    }

    printf("Maximum value: %.6f\n", max_val);
    printf("Minimum value: %.6f\n", min_val);

    return 0;
}