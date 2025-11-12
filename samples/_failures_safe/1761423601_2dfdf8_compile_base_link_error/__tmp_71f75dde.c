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
            double harmonic_term = 1.0 / (k + 1);
            harmonic_sum += harmonic_term;
            printf("Term %d: %.6f\n", k + 1, harmonic_term);
        }
        k++;
    } while (k < n);
    printf("Harmonic sum: %.6f\n", harmonic_sum);
    
    printf("\nNested loops (multiplication table):\n");
    int table_size = (n > 10) ? 10 : n;
    for (int row = 1; row <= table_size; row++) {
        for (int col = 1; col <= table_size; col++) {
            printf("%4d", row * col);
        }
        printf("\n");
    }
    
    printf("\nBreak and continue example:\n");
    int prime_count = 0;
    for (int num = 2; num <= MAX_ITERATIONS && prime_count < n; num++) {
        int is_prime = 1;
        if (num > 2 && num % 2 == 0) {
            continue;
        }
        for (int div = 3; div * div <= num; div += 2) {
            if (num % div == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime) {
            printf("Prime %d: %d\n", ++prime_count, num);
        }
    }
    
    double final_sum = 0.0;
    for (int i = 0; i < n; i++) {
        final_sum += sequence[i];
    }
    
    printf("\nFinal arithmetic sequence sum: %.2f\n", final_sum);
    printf("All computations completed successfully.\n");
    
    return 0;
}