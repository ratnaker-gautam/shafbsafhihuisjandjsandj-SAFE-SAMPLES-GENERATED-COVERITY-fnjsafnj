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
    harmonic_mean = n / harmonic_mean;
    
    uint64_t fibonacci_prev = 0;
    uint64_t fibonacci_curr = 1;
    for (int i = 2; i <= n; i++) {
        if (fibonacci_curr > UINT64_MAX - fibonacci_prev) {
            fprintf(stderr, "Error: Fibonacci overflow detected\n");
            return 1;
        }
        uint64_t next = fibonacci_prev + fibonacci_curr;
        fibonacci_prev = fibonacci_curr;
        fibonacci_curr = next;
    }
    
    double pi_approximation = 0.0;
    int iterations = (n < MAX_ITERATIONS) ? n : MAX_ITERATIONS;
    for (int k = 0; k < iterations; k++) {
        double term = (k % 2 == 0) ? 1.0 : -1.0;
        term /= (2 * k + 1);
        pi_approximation += term;
    }
    pi_approximation *= 4.0;
    
    printf("Results for n = %d:\n", n);
    printf("Sum of first %d integers: %lu\n", n, (unsigned long)sum);
    printf("Product of first %d integers: %lu\n", n, (unsigned long)product);
    printf("Harmonic mean of first %d integers: %.6f\n", n, harmonic_mean);
    printf("Fibonacci(%d): %lu\n", n, (unsigned long)fibonacci_curr);
    printf("Pi approximation (Leibniz): %.10f\n", pi_approximation);
    
    return 0;
}