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
    int valid_product = 1;
    
    for (int i = 1; i <= n; i++) {
        sum += i;
        
        if (valid_product) {
            if (i > UINT64_MAX / product) {
                valid_product = 0;
            } else {
                product *= i;
            }
        }
    }
    
    double harmonic_mean = 0.0;
    for (int i = 1; i <= n; i++) {
        harmonic_mean += 1.0 / i;
    }
    harmonic_mean = n / harmonic_mean;
    
    uint64_t sum_squares = 0;
    for (int i = 1; i <= n; i++) {
        if (i > UINT64_MAX / i) {
            fprintf(stderr, "Error: Overflow in square calculation\n");
            return 1;
        }
        uint64_t square = (uint64_t)i * i;
        if (square > UINT64_MAX - sum_squares) {
            fprintf(stderr, "Error: Overflow in sum of squares\n");
            return 1;
        }
        sum_squares += square;
    }
    
    printf("Input: %d\n", n);
    printf("Sum of first %d integers: %lu\n", n, (unsigned long)sum);
    
    if (valid_product) {
        printf("Product of first %d integers: %lu\n", n, (unsigned long)product);
    } else {
        printf("Product of first %d integers: overflow occurred\n", n);
    }
    
    printf("Harmonic mean of first %d integers: %.6f\n", n, harmonic_mean);
    printf("Sum of squares of first %d integers: %lu\n", n, (unsigned long)sum_squares);
    
    double pi_approximation = 0.0;
    int iterations = (n < MAX_ITERATIONS) ? n : MAX_ITERATIONS;
    for (int k = 0; k < iterations; k++) {
        double term = (k % 2 == 0) ? 1.0 : -1.0;
        term /= (2.0 * k + 1.0);
        pi_approximation += term;
    }
    pi_approximation *= 4.0;
    
    printf("Pi approximation using %d terms: %.10f\n", iterations, pi_approximation);
    
    return 0;
}