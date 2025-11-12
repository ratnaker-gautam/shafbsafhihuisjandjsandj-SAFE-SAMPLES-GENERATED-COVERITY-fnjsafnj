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
        
        if (i % n == 0 && i > 0) {
            long double current_pi = pi_approx * 4.0L;
            printf("After %d iterations: %.15Lf\n", i, current_pi);
        }
    }
    
    long double final_pi = pi_approx * 4.0L;
    printf("Final approximation of pi: %.15Lf\n", final_pi);
    printf("Total iterations: %d\n", MAX_ITERATIONS);
    
    return 0;
}