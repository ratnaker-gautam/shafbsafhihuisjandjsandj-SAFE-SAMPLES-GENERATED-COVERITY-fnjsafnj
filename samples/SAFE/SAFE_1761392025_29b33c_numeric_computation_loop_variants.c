//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000

int main() {
    int n;
    printf("Enter number of terms to compute (1-%d): ", MAX_ITERATIONS);
    
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (n < 1 || n > MAX_ITERATIONS) {
        fprintf(stderr, "Error: Number must be between 1 and %d\n", MAX_ITERATIONS);
        return 1;
    }
    
    double sum = 0.0;
    double term;
    int i = 0;
    
    printf("Computing alternating harmonic series for %d terms:\n", n);
    
    while (i < n) {
        term = 1.0 / (i + 1);
        if (i % 2 == 1) {
            term = -term;
        }
        sum += term;
        i++;
    }
    
    printf("Result using while loop: %.10f\n", sum);
    
    sum = 0.0;
    int j;
    
    for (j = 0; j < n; j++) {
        term = 1.0 / (j + 1);
        if (j % 2 == 1) {
            term = -term;
        }
        sum += term;
    }
    
    printf("Result using for loop: %.10f\n", sum);
    
    sum = 0.0;
    int k = 0;
    
    do {
        term = 1.0 / (k + 1);
        if (k % 2 == 1) {
            term = -term;
        }
        sum += term;
        k++;
    } while (k < n);
    
    printf("Result using do-while loop: %.10f\n", sum);
    
    return 0;
}