//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000000

int main() {
    int n;
    printf("Enter a positive integer (1-1000): ");
    
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (n < 1 || n > 1000) {
        fprintf(stderr, "Error: Number must be between 1 and 1000\n");
        return 1;
    }
    
    if (n > INT_MAX / 1000) {
        fprintf(stderr, "Error: Input too large for safe computation\n");
        return 1;
    }
    
    uint64_t sum = 0;
    uint64_t product = 1;
    
    for (int i = 1; i <= n; i++) {
        if (i > 0 && product > UINT64_MAX / i) {
            fprintf(stderr, "Error: Integer overflow detected\n");
            return 1;
        }
        product *= i;
        sum += i;
    }
    
    double harmonic_mean = 0.0;
    for (int i = 1; i <= n; i++) {
        if (i == 0) continue;
        harmonic_mean += 1.0 / i;
    }
    if (n > 0 && harmonic_mean != 0.0) {
        harmonic_mean = n / harmonic_mean;
    }
    
    double pi_approx = 0.0;
    for (int iter = 0; iter < MAX_ITERATIONS; iter++) {
        double term = (iter % 2 == 0) ? 1.0 : -1.0;
        term /= (2 * iter + 1);
        pi_approx += term;
    }
    pi_approx *= 4.0;
    
    printf("Sum of first %d integers: %lu\n", n, sum);
    printf("Product of first %d integers: %lu\n", n, product);
    printf("Harmonic mean of first %d integers: %.6f\n", n, harmonic_mean);
    printf("Pi approximation (Leibniz): %.10f\n", pi_approx);
    
    return 0;
}