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
    
    printf("Computing sequence using different loop variants:\n\n");
    
    printf("For loop (arithmetic sequence):\n");
    for (int i = 0; i < n; i++) {
        sequence[i] = (i + 1) * 2.5;
        printf("Term %d: %.2f\n", i + 1, sequence[i]);
    }
    
    printf("\nWhile loop (harmonic series):\n");
    int j = 0;
    double harmonic_sum = 0.0;
    while (j < n) {
        if (j == 0) {
            sequence[j] = 1.0;
        } else {
            sequence[j] = 1.0 / (j + 1);
        }
        harmonic_sum += sequence[j];
        printf("Term %d: %.6f\n", j + 1, sequence[j]);
        j++;
    }
    printf("Harmonic sum: %.6f\n", harmonic_sum);
    
    printf("\nDo-while loop (geometric sequence):\n");
    int k = 0;
    double geometric_product = 1.0;
    do {
        sequence[k] = pow(2.0, k);
        geometric_product *= sequence[k];
        printf("Term %d: %.2f\n", k + 1, sequence[k]);
        k++;
    } while (k < n);
    printf("Geometric product: %.2f\n", geometric_product);
    
    printf("\nNested loops (matrix-like computation):\n");
    int rows = (n < 10) ? n : 10;
    int cols = (n < 10) ? n : 10;
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            double value = sin(i * 0.5) * cos(j * 0.3);
            printf("%8.4f ", value);
        }
        printf("\n");
    }
    
    printf("\nBreak and continue example:\n");
    int count = 0;
    for (int i = 1; i <= MAX_ITERATIONS && count < n; i++) {
        if (i % 7 == 0) {
            continue;
        }
        if (i > 50 && count > n / 2) {
            break;
        }
        printf("Value %d: %d\n", count + 1, i);
        count++;
    }
    
    printf("\nFinal statistics:\n");
    double min_val = sequence[0];
    double max_val = sequence[0];
    double total = 0.0;
    
    for (int i = 0; i < n; i++) {
        if (sequence[i] < min_val) min_val = sequence[i];
        if (sequence[i] > max_val) max_val = sequence[i];
        total += sequence[i];
    }
    
    printf("Minimum: %.6f\n", min_val);
    printf("Maximum: %.6f\n", max_val);
    printf("Average: %.6f\n", total / n);
    
    return 0;
}