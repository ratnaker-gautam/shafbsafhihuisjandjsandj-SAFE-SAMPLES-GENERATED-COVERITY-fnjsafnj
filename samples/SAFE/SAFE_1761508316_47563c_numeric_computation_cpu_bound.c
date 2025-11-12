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
    int sign = 1;
    
    for (int i = 0; i < MAX_ITERATIONS; i++) {
        long double term = 1.0L / (2 * i + 1);
        if (sign > 0) {
            pi_approx += term;
        } else {
            pi_approx -= term;
        }
        sign = -sign;
    }
    
    pi_approx *= 4.0L;
    
    long double result = 0.0L;
    for (int i = 1; i <= n; i++) {
        long double sum = 0.0L;
        for (int j = 1; j <= i; j++) {
            sum += (long double)j / (long double)(i + 1);
        }
        result += sum * pi_approx / (long double)(n);
    }
    
    printf("Computed value: %.15Lf\n", result);
    
    long double verification = 0.0L;
    for (int i = 0; i < 10000; i++) {
        long double x = (long double)i / 10000.0L;
        verification += x * x;
    }
    verification /= 10000.0L;
    
    printf("Verification constant: %.15Lf\n", verification);
    
    return 0;
}