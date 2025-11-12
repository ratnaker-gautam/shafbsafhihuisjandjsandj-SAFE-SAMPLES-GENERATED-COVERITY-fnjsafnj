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
    
    if (n > INT_MAX / 2) {
        fprintf(stderr, "Error: Input too large for safe computation\n");
        return 1;
    }
    
    long long sum = 0;
    long long product = 1;
    int valid_product = 1;
    
    for (int i = 1; i <= n; i++) {
        sum += i;
        
        if (valid_product) {
            if (i > LLONG_MAX / product) {
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
    
    long long factorial_approx = 1;
    for (int i = 1; i <= n && i <= 20; i++) {
        if (i > LLONG_MAX / factorial_approx) {
            break;
        }
        factorial_approx *= i;
    }
    
    double pi_approx = 0.0;
    int iterations = (n < MAX_ITERATIONS) ? n : MAX_ITERATIONS;
    for (int k = 0; k < iterations; k++) {
        double term = (k % 2 == 0) ? 1.0 : -1.0;
        term /= (2.0 * k + 1.0);
        pi_approx += term;
    }
    pi_approx *= 4.0;
    
    printf("Sum of first %d integers: %lld\n", n, sum);
    printf("Product of first %d integers: ", n);
    if (valid_product) {
        printf("%lld\n", product);
    } else {
        printf("overflow\n");
    }
    printf("Harmonic mean of first %d integers: %.6f\n", n, harmonic_mean);
    printf("Factorial approximation (up to 20!): %lld\n", factorial_approx);
    printf("Pi approximation (%d terms): %.10f\n", iterations, pi_approx);
    
    return 0;
}