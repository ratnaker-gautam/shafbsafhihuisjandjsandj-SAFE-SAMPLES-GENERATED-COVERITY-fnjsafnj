//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000

int main(void) {
    int n;
    printf("Enter number of terms to compute (1-%d): ", MAX_ITERATIONS);
    
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (n < 1 || n > MAX_ITERATIONS) {
        fprintf(stderr, "Error: Number out of range\n");
        return 1;
    }
    
    double sum = 0.0;
    double term;
    int i = 0;
    
    printf("Computing series approximation of pi/4 using %d terms\n", n);
    
    while (i < n) {
        term = 1.0 / (2 * i + 1);
        if (i % 2 == 1) {
            term = -term;
        }
        sum += term;
        i++;
    }
    
    double pi_approx = sum * 4.0;
    printf("Approximation of pi: %.10f\n", pi_approx);
    
    double running_sum = 0.0;
    double max_term = 0.0;
    double min_term = 0.0;
    
    for (int j = 0; j < n; j++) {
        term = 1.0 / (2 * j + 1);
        if (j % 2 == 1) {
            term = -term;
        }
        running_sum += term;
        
        if (j == 0) {
            max_term = term;
            min_term = term;
        } else {
            if (term > max_term) max_term = term;
            if (term < min_term) min_term = term;
        }
    }
    
    printf("Verification sum: %.10f\n", running_sum);
    printf("Largest term: %.10f\n", max_term);
    printf("Smallest term: %.10f\n", min_term);
    
    double product = 1.0;
    int k = 1;
    
    do {
        if (k > n) break;
        double factor = 1.0 + (1.0 / (k * k));
        if (factor > 0 && product < __DBL_MAX__ / factor) {
            product *= factor;
        } else {
            fprintf(stderr, "Warning: Product overflow at term %d\n", k);
            break;
        }
        k++;
    } while (k <= n);
    
    printf("Wallis product after %d terms: %.10f\n", k-1, product);
    
    double final_result = pi_approx * product;
    printf("Combined result: %.10f\n", final_result);
    
    return 0;
}