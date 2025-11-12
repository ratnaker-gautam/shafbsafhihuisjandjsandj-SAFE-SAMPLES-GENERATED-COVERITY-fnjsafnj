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
    
    printf("Computing sequence using different loop styles...\n\n");
    
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
        }
        k++;
    } while (k < n);
    
    printf("\nNested loops (matrix-like computation):\n");
    int rows = (n < 10) ? n : 10;
    int cols = (n + rows - 1) / rows;
    
    int count = 0;
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (count < n) {
                double term = sin((double)(count + 1) * 0.1) * exp((double)(count + 1) * 0.05);
                printf("%8.4f ", term);
                sum += term;
                count++;
            }
        }
        printf("\n");
    }
    
    printf("\nFinal sum of all computed terms: %.6f\n", sum);
    
    double average = sum / n;
    printf("Average value: %.6f\n", average);
    
    double variance = 0.0;
    for (int i = 0; i < n; i++) {
        double diff = sequence[i] - average;
        variance += diff * diff;
    }
    variance /= n;
    
    printf("Variance: %.6f\n", variance);
    printf("Standard deviation: %.6f\n", sqrt(variance));
    
    return 0;
}