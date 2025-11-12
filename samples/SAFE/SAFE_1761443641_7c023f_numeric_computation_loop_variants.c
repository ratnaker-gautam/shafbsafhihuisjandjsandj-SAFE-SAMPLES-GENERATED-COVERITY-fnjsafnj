//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

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

    printf("Computing harmonic series partial sums...\n");
    
    int i = 0;
    double sum = 0.0;
    
    while (i < n) {
        if (i >= ARRAY_SIZE) break;
        sum += 1.0 / (i + 1);
        results[i] = sum;
        i++;
    }

    printf("Results using while loop:\n");
    for (int j = 0; j < i; j++) {
        printf("H(%d) = %.6f\n", j + 1, results[j]);
    }

    printf("\nComputing alternating harmonic series...\n");
    double alt_sum = 0.0;
    
    for (int k = 0; k < n; k++) {
        if (k >= ARRAY_SIZE) break;
        double term = 1.0 / (k + 1);
        if (k % 2 == 1) {
            term = -term;
        }
        alt_sum += term;
        printf("A(%d) = %.6f\n", k + 1, alt_sum);
    }

    printf("\nComputing geometric progression...\n");
    double geometric[ARRAY_SIZE];
    double ratio = 0.5;
    double current = 1.0;
    int count = 0;
    
    do {
        if (count >= ARRAY_SIZE || count >= n) break;
        geometric[count] = current;
        current *= ratio;
        count++;
    } while (count < n && count < ARRAY_SIZE);

    for (int m = 0; m < count; m++) {
        printf("G(%d) = %.6f\n", m + 1, geometric[m]);
    }

    printf("\nComputing factorial approximations...\n");
    double factorial_approx[ARRAY_SIZE];
    double fact = 1.0;
    
    for (int p = 0; p < n && p < ARRAY_SIZE; p++) {
        if (p > 0) {
            if (fact > __DBL_MAX__ / (p + 1)) {
                fprintf(stderr, "Warning: Factorial approximation overflow at term %d\n", p + 1);
                break;
            }
            fact *= (p + 1);
        }
        factorial_approx[p] = fact;
        printf("F(%d) = %.0f\n", p + 1, factorial_approx[p]);
    }

    return 0;
}