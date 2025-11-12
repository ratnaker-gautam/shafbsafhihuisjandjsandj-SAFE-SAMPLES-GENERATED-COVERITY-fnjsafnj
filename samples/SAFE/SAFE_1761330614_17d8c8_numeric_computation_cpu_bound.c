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
    
    long double pi_approx = 0.0L;
    int iterations = (n * 1000 > MAX_ITERATIONS) ? MAX_ITERATIONS : n * 1000;
    
    if (iterations < 1) {
        iterations = 1;
    }
    
    for (int i = 0; i < iterations; i++) {
        long double term = 1.0L / (2 * i + 1);
        if (i % 2 == 0) {
            pi_approx += term;
        } else {
            pi_approx -= term;
        }
    }
    
    pi_approx *= 4.0L;
    
    long double sqrt_approx = 0.0L;
    if (n > 0) {
        long double x = (long double)n;
        for (int i = 0; i < 50; i++) {
            if (x <= 0.0L) {
                break;
            }
            sqrt_approx = 0.5L * (x + (long double)n / x);
            if (sqrt_approx == x) {
                break;
            }
            x = sqrt_approx;
        }
    }
    
    unsigned long long factorial = 1;
    if (n <= 20) {
        for (int i = 1; i <= n; i++) {
            if (factorial > ULLONG_MAX / i) {
                factorial = ULLONG_MAX;
                break;
            }
            factorial *= i;
        }
    } else {
        factorial = ULLONG_MAX;
    }
    
    printf("Input: %d\n", n);
    printf("Pi approximation (Leibniz): %.15Lf\n", pi_approx);
    printf("Square root approximation: %.15Lf\n", sqrt_approx);
    if (n <= 20) {
        printf("Factorial: %llu\n", factorial);
    } else {
        printf("Factorial: overflow (input > 20)\n");
    }
    
    return 0;
}