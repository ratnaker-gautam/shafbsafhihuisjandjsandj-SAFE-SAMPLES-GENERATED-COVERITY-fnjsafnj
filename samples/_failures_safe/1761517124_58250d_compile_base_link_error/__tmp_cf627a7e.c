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
    
    printf("Results for n = %d:\n", n);
    printf("Sum of first %d integers: %lu\n", n, sum);
    
    if (valid_product) {
        printf("Product of first %d integers: %lu\n", n, product);
    } else {
        printf("Product: too large to compute\n");
    }
    
    printf("Harmonic mean: %.6f\n", harmonic_mean);
    printf("Sum of squares: %lu\n", sum_squares);
    
    double geometric_mean = 1.0;
    for (int i = 1; i <= n; i++) {
        geometric_mean *= i;
    }
    geometric_mean = pow(geometric_mean, 1.0 / n);
    printf("Geometric mean: %.6f\n", geometric_mean);
    
    return 0;
}