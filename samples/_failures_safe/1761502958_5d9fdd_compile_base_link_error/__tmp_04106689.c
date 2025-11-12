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
        fprintf(stderr, "Error: Number out of range\n");
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
    
    printf("\nWhile loop (geometric sequence):\n");
    int j = 0;
    double geometric_sum = 0.0;
    while (j < n) {
        double term = pow(1.5, j + 1);
        geometric_sum += term;
        printf("Term %d: %.4f\n", j + 1, term);
        j++;
    }
    printf("Geometric sum: %.4f\n", geometric_sum);
    
    printf("\nDo-while loop (harmonic series):\n");
    int k = 0;
    double harmonic_sum = 0.0;
    do {
        if (k < n) {
            double term = 1.0 / (k + 1);
            harmonic_sum += term;
            printf("Term %d: %.4f\n", k + 1, term);
        }
        k++;
    } while (k < n);
    printf("Harmonic sum: %.4f\n", harmonic_sum);
    
    printf("\nNested loops (matrix-like computation):\n");
    int dimension = (n > 10) ? 10 : n;
    for (int row = 0; row < dimension; row++) {
        for (int col = 0; col < dimension; col++) {
            double value = sin((double)(row + 1)) * cos((double)(col + 1));
            printf("%8.4f ", value);
        }
        printf("\n");
    }
    
    printf("\nBreak/continue example (primes up to %d):\n", n * 2);
    int prime_count = 0;
    for (int num = 2; num <= n * 2 && prime_count < n; num++) {
        int is_prime = 1;
        for (int div = 2; div * div <= num; div++) {
            if (num % div == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime) {
            printf("Prime %d: %d\n", ++prime_count, num);
        }
    }
    
    printf("\nComputing final statistics:\n");
    double min_val = sequence[0];
    double max_val = sequence[0];
    double running_sum = 0.0;
    
    int idx = 0;
    while (idx < n) {
        if (sequence[idx] < min_val) min_val = sequence[idx];
        if (sequence[idx] > max_val) max_val = sequence[idx];
        running_sum += sequence[idx];
        idx++;
    }
    
    double average = running_sum / n;
    printf("Min: %.2f, Max: %.2f, Average: %.2f\n", min_val, max_val, average);
    
    printf("\nFinal computation complete.\n");
    return 0;
}