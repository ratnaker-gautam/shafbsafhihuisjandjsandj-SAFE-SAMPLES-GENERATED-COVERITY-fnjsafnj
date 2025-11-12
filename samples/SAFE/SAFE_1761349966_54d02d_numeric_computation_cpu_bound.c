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
    int sign = 1;
    
    for (int i = 0; i < MAX_ITERATIONS; i++) {
        if (i > INT_MAX - 1000000) {
            break;
        }
        
        long double term = 1.0L / (2 * i + 1);
        if (sign > 0) {
            pi_approx += term;
        } else {
            pi_approx -= term;
        }
        
        sign = -sign;
        
        if (i % 100000 == 0 && i > 0) {
            if (pi_approx > 1e308L || pi_approx < -1e308L) {
                fprintf(stderr, "Error: Numerical overflow detected\n");
                return 1;
            }
        }
    }
    
    pi_approx *= 4.0L;
    
    long double result = 0.0L;
    for (int i = 1; i <= n; i++) {
        if (i > INT_MAX / i) {
            fprintf(stderr, "Error: Integer overflow detected\n");
            return 1;
        }
        
        long double term = (long double)(i * i) / (long double)(n * n);
        if (term > 1e308L / n) {
            fprintf(stderr, "Error: Numerical overflow detected\n");
            return 1;
        }
        
        result += term;
    }
    
    result = result * pi_approx / n;
    
    if (result != result) {
        fprintf(stderr, "Error: Numerical computation resulted in NaN\n");
        return 1;
    }
    
    if (result > 1e308L || result < -1e308L) {
        fprintf(stderr, "Error: Result out of representable range\n");
        return 1;
    }
    
    printf("Computed result: %.15Lf\n", result);
    printf("Input value: %d\n", n);
    
    return 0;
}