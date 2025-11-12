//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <float.h>

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
        if (i > INT_MAX / 2 - 1) {
            break;
        }
        
        long double denominator = 2.0L * i + 1;
        if (denominator == 0.0L) {
            continue;
        }
        long double term = 1.0L / denominator;
        
        if (sign > 0) {
            if (pi_approx > LDBL_MAX - term) {
                break;
            }
            pi_approx += term;
        } else {
            if (pi_approx < -LDBL_MAX + term) {
                break;
            }
            pi_approx -= term;
        }
        sign = -sign;
    }
    
    if (pi_approx > LDBL_MAX / 4.0L) {
        pi_approx = LDBL_MAX;
    } else {
        pi_approx *= 4.0L;
    }
    
    long double result = 0.0L;
    for (int i = 1; i <= n; i++) {
        if (i > INT_MAX / i) {
            break;
        }
        long double divisor = (long double)(i * i);
        if (divisor == 0.0L) {
            continue;
        }
        long double term = pi_approx / divisor;
        
        if (result > LDBL_MAX - term) {
            break;
        }
        result += term;
    }
    
    printf("Computed value: %.15Lf\n", result);
    printf("Input multiplier: %d\n", n);
    
    return 0;
}