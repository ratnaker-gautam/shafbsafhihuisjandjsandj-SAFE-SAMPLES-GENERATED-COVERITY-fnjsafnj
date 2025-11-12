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
    
    printf("Computing sequence using different loop styles...\n");
    
    int i = 0;
    while (i < n) {
        double term = 1.0 / ((i + 1) * (i + 1));
        sequence[i] = term;
        sum += term;
        i++;
    }
    
    double pi_approximation = sqrt(6.0 * sum);
    printf("While loop result: pi ≈ %.10f\n", pi_approximation);
    
    sum = 0.0;
    for (int j = 0; j < n; j++) {
        double term = 1.0 / ((j + 1) * (j + 1));
        sum += term;
    }
    
    pi_approximation = sqrt(6.0 * sum);
    printf("For loop result: pi ≈ %.10f\n", pi_approximation);
    
    sum = 0.0;
    int k = 0;
    do {
        if (k >= n) break;
        double term = 1.0 / ((k + 1) * (k + 1));
        sum += term;
        k++;
    } while (k < n);
    
    pi_approximation = sqrt(6.0 * sum);
    printf("Do-while loop result: pi ≈ %.10f\n", pi_approximation);
    
    double max_val = sequence[0];
    double min_val = sequence[0];
    
    for (int idx = 1; idx < n; idx++) {
        if (sequence[idx] > max_val) {
            max_val = sequence[idx];
        }
        if (sequence[idx] < min_val) {
            min_val = sequence[idx];
        }
    }
    
    printf("Sequence statistics: max=%.6f, min=%.6f\n", max_val, min_val);
    
    double running_sum = 0.0;
    printf("Running sums: ");
    for (int m = 0; m < n; m++) {
        running_sum += sequence[m];
        printf("%.4f", running_sum);
        if (m < n - 1) {
            printf(", ");
        }
    }
    printf("\n");
    
    return 0;
}