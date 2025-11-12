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
            if (i > UINT64_MAX / product) {
                valid_product = 0;
            } else {
                product *= i;
            }
        }
    }
    
    double harmonic_sum = 0.0;
    for (int i = 1; i <= n; i++) {
        if (i == 0) continue;
        harmonic_sum += 1.0 / i;
    }
    
    printf("Sum of first %d natural numbers: %lu\n", n, sum);
    
    if (valid_product) {
        printf("Product of first %d natural numbers: %lu\n", n, product);
    } else {
        printf("Product of first %d natural numbers: overflow occurred\n", n);
    }
    
    printf("Harmonic sum of first %d natural numbers: %.10f\n", n, harmonic_sum);
    
    uint64_t fibonacci[3] = {0, 1, 1};
    if (n >= 1) {
        for (int i = 3; i <= n && i < 94; i++) {
            if (fibonacci[2] > UINT64_MAX - fibonacci[1]) {
                printf("Fibonacci sequence overflow at position %d\n", i);
                break;
            }
            fibonacci[0] = fibonacci[1];
            fibonacci[1] = fibonacci[2];
            fibonacci[2] = fibonacci[0] + fibonacci[1];
        }
        printf("Fibonacci number at position %d: %lu\n", n, (n == 0) ? 0 : (n == 1) ? 1 : fibonacci[2]);
    }
    
    return 0;
}