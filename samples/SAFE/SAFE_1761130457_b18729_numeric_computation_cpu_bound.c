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
    
    double harmonic_sum = 0.0;
    for (int i = 1; i <= n; i++) {
        harmonic_sum += 1.0 / i;
    }
    
    uint64_t fibonacci_prev = 0;
    uint64_t fibonacci_curr = 1;
    
    if (n >= 1) {
        for (int i = 2; i <= n; i++) {
            uint64_t next = fibonacci_prev + fibonacci_curr;
            if (next < fibonacci_curr) {
                fprintf(stderr, "Error: Fibonacci overflow at position %d\n", i);
                return 1;
            }
            fibonacci_prev = fibonacci_curr;
            fibonacci_curr = next;
        }
    }
    
    printf("Sum of first %d integers: %lu\n", n, sum);
    printf("Harmonic sum of first %d integers: %.6f\n", n, harmonic_sum);
    
    if (valid_product) {
        printf("Product of first %d integers: %lu\n", n, product);
    } else {
        printf("Product of first %d integers: overflow occurred\n", n);
    }
    
    printf("Fibonacci number at position %d: %lu\n", n, fibonacci_curr);
    
    double pi_approximation = 0.0;
    int iterations = (n < MAX_ITERATIONS) ? n : MAX_ITERATIONS;
    
    for (int k = 0; k < iterations; k++) {
        double term = (k % 2 == 0) ? 1.0 : -1.0;
        term /= (2 * k + 1);
        pi_approximation += term;
    }
    pi_approximation *= 4.0;
    
    printf("Pi approximation using %d terms: %.10f\n", iterations, pi_approximation);
    
    return 0;
}