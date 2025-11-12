//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: numeric_computation
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
            if (product > UINT64_MAX / i) {
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
    
    uint64_t factorial = 1;
    for (int i = 1; i <= n; i++) {
        if (factorial > UINT64_MAX / i) {
            factorial = UINT64_MAX;
            break;
        }
        factorial *= i;
    }
    
    printf("Sum of first %d integers: %lu\n", n, sum);
    printf("Product of first %d integers: ", n);
    if (valid_product) {
        printf("%lu\n", product);
    } else {
        printf("overflow\n");
    }
    printf("Harmonic sum of first %d integers: %.10f\n", n, harmonic_sum);
    printf("Factorial of %d: ", n);
    if (factorial != UINT64_MAX || n <= 20) {
        printf("%lu\n", factorial);
    } else {
        printf("overflow\n");
    }
    
    double pi_approx = 0.0;
    int iterations = (n < MAX_ITERATIONS) ? n : MAX_ITERATIONS;
    for (int i = 0; i < iterations; i++) {
        double term = (i % 2 == 0) ? 1.0 : -1.0;
        term /= (2 * i + 1);
        pi_approx += term;
    }
    pi_approx *= 4.0;
    
    printf("Pi approximation using %d terms: %.10f\n", iterations, pi_approx);
    
    return 0;
}