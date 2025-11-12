//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000
#define ARRAY_SIZE 100

int main(void) {
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
    
    int i = 0;
    while (i < n) {
        double sum = 0.0;
        int j;
        for (j = 1; j <= (i + 1); j++) {
            if (j == 0) continue;
            double term = 1.0 / (double)j;
            if (term < 0.0000001) break;
            sum += term;
        }
        results[i] = sum;
        i++;
    }
    
    double total = 0.0;
    int k;
    for (k = 0; k < n; k++) {
        total += results[k];
    }
    
    double average = total / n;
    
    printf("Harmonic series partial sums computed\n");
    printf("Average of %d partial sums: %.6f\n", n, average);
    
    double variance = 0.0;
    int m = 0;
    do {
        if (m >= n) break;
        double diff = results[m] - average;
        variance += diff * diff;
        m++;
    } while (m < n);
    
    if (n > 1) {
        variance /= (n - 1);
        printf("Variance: %.6f\n", variance);
    } else {
        printf("Variance: undefined (n=1)\n");
    }
    
    return 0;
}