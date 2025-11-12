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
    
    long double pi_approx = 0.0L;
    int iterations = (n > MAX_ITERATIONS) ? MAX_ITERATIONS : n;
    
    for (int i = 0; i < iterations; i++) {
        long double term = 1.0L / (2 * (long double)i + 1);
        if (i % 2 == 0) {
            pi_approx += term;
        } else {
            pi_approx -= term;
        }
    }
    
    pi_approx *= 4.0L;
    
    long double error_bound = 1.0L / (2 * (long double)iterations + 1);
    
    printf("Pi approximation after %d iterations: %.15Lf\n", iterations, pi_approx);
    printf("Error bound: %.15Lf\n", error_bound);
    
    long double sum_check = 0.0L;
    for (int i = 1; i <= 1000; i++) {
        sum_check += 1.0L / (long double)(i * i);
    }
    
    printf("Sum of 1/k^2 for k=1 to 1000: %.15Lf\n", sum_check);
    
    unsigned long long factorial = 1;
    if (n <= 20) {
        for (int i = 1; i <= n; i++) {
            if (factorial > ULLONG_MAX / (unsigned long long)i) {
                fprintf(stderr, "Warning: Factorial overflow at i=%d\n", i);
                factorial = 0;
                break;
            }
            factorial *= (unsigned long long)i;
        }
        if (factorial != 0) {
            printf("%d! = %llu\n", n, factorial);
        }
    } else {
        printf("Factorial skipped for n > 20 (would overflow)\n");
    }
    
    return 0;
}