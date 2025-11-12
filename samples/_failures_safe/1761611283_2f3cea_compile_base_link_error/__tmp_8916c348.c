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
    
    double results[ARRAY_SIZE];
    
    int i = 0;
    while (i < n) {
        double sum = 0.0;
        int j;
        for (j = 1; j <= i + 1; j++) {
            if (j == 0) continue;
            double term = 1.0 / (j * j);
            if (!isfinite(term)) {
                fprintf(stderr, "Error: Numerical overflow\n");
                return 1;
            }
            sum += term;
        }
        results[i] = sum;
        i++;
    }
    
    double final_sum = 0.0;
    int k = 0;
    do {
        if (k >= n) break;
        final_sum += results[k];
        if (!isfinite(final_sum)) {
            fprintf(stderr, "Error: Numerical overflow in final sum\n");
            return 1;
        }
        k++;
    } while (k < n);
    
    double harmonic_mean;
    if (n > 0 && final_sum > 0.0) {
        harmonic_mean = n / final_sum;
        if (!isfinite(harmonic_mean)) {
            fprintf(stderr, "Error: Numerical overflow in harmonic mean\n");
            return 1;
        }
    } else {
        harmonic_mean = 0.0;
    }
    
    printf("Computed harmonic mean of partial sums: %.10f\n", harmonic_mean);
    
    double pi_approximation = sqrt(6.0 * results[n-1]);
    if (!isfinite(pi_approximation)) {
        fprintf(stderr, "Error: Numerical overflow in pi approximation\n");
        return 1;
    }
    
    printf("Pi approximation from last term: %.10f\n", pi_approximation);
    
    return 0;
}