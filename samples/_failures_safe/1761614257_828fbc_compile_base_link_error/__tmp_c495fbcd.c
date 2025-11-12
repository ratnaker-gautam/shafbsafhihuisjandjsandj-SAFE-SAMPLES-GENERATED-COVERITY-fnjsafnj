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
    
    printf("Computing sequence using different loop styles:\n\n");
    
    printf("For loop (arithmetic sequence):\n");
    for (int i = 0; i < n; i++) {
        sequence[i] = (i + 1) * 2.5;
        printf("Term %d: %.2f\n", i + 1, sequence[i]);
    }
    
    printf("\nWhile loop (geometric sequence):\n");
    int j = 0;
    double value = 1.0;
    while (j < n) {
        sequence[j] = value;
        printf("Term %d: %.2f\n", j + 1, sequence[j]);
        value *= 1.5;
        j++;
    }
    
    printf("\nDo-while loop (harmonic series):\n");
    int k = 0;
    do {
        if (k < n) {
            sequence[k] = 1.0 / (k + 1);
            printf("Term %d: %.6f\n", k + 1, sequence[k]);
            k++;
        }
    } while (k < n);
    
    printf("\nNested loops (matrix-like computation):\n");
    int rows = (int)sqrt(n);
    if (rows < 1) rows = 1;
    int cols = n / rows;
    if (cols < 1) cols = 1;
    
    int count = 0;
    for (int r = 0; r < rows && count < n; r++) {
        for (int c = 0; c < cols && count < n; c++) {
            sequence[count] = (r + 1) * (c + 1) * 0.1;
            printf("Element (%d,%d): %.2f\n", r + 1, c + 1, sequence[count]);
            count++;
        }
    }
    
    printf("\nComputing statistics:\n");
    
    double min_val = sequence[0];
    double max_val = sequence[0];
    sum = 0.0;
    
    for (int i = 0; i < n; i++) {
        if (sequence[i] < min_val) min_val = sequence[i];
        if (sequence[i] > max_val) max_val = sequence[i];
        sum += sequence[i];
    }
    
    double mean = sum / n;
    
    double variance = 0.0;
    int m = 0;
    while (m < n) {
        double diff = sequence[m] - mean;
        variance += diff * diff;
        m++;
    }
    variance /= n;
    
    printf("Statistics for %d terms:\n", n);
    printf("Minimum: %.6f\n", min_val);
    printf("Maximum: %.6f\n", max_val);
    printf("Mean: %.6f\n", mean);
    printf("Variance: %.6f\n", variance);
    
    return 0;
}