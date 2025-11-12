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
    
    if (n > INT_MAX / 100) {
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
    
    double geometric_mean = 1.0;
    for (int i = 1; i <= n; i++) {
        geometric_mean *= i;
    }
    if (n > 0) {
        geometric_mean = pow(geometric_mean, 1.0 / n);
    }
    
    printf("Results for n = %d:\n", n);
    printf("Sum of first %d integers: %lu\n", n, (unsigned long)sum);
    
    if (valid_product) {
        printf("Product of first %d integers: %lu\n", n, (unsigned long)product);
    } else {
        printf("Product of first %d integers: overflow occurred\n", n);
    }
    
    printf("Harmonic sum of first %d integers: %.10f\n", n, harmonic_sum);
    printf("Geometric mean of first %d integers: %.10f\n", n, geometric_mean);
    
    uint64_t fibonacci_prev = 0;
    uint64_t fibonacci_curr = 1;
    uint64_t fibonacci_next;
    
    printf("Fibonacci sequence up to %d terms:\n", n > 20 ? 20 : n);
    int terms_to_print = n > 20 ? 20 : n;
    
    for (int i = 0; i < terms_to_print; i++) {
        if (i == 0) {
            printf("%lu", (unsigned long)fibonacci_prev);
        } else {
            printf(", %lu", (unsigned long)fibonacci_prev);
        }
        
        if (fibonacci_prev > UINT64_MAX - fibonacci_curr) {
            printf("\n(overflow at term %d)\n", i + 1);
            break;
        }
        
        fibonacci_next = fibonacci_prev + fibonacci_curr;
        fibonacci_prev = fibonacci_curr;
        fibonacci_curr = fibonacci_next;
    }
    printf("\n");
    
    return 0;
}