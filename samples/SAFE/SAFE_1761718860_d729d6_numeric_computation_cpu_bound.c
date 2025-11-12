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
    
    long long sum = 0;
    int iterations = n * 1000;
    
    if (iterations > MAX_ITERATIONS) {
        iterations = MAX_ITERATIONS;
    }
    
    for (int i = 1; i <= iterations; i++) {
        if (i % 2 == 0) {
            sum += i;
        } else {
            sum -= i;
        }
        
        if (sum > LLONG_MAX / 2 || sum < LLONG_MIN / 2) {
            fprintf(stderr, "Error: Computation would overflow\n");
            return 1;
        }
    }
    
    double result = 0.0;
    for (int i = 1; i <= n; i++) {
        double term = 1.0 / (double)i;
        result += term;
        
        if (result > 1e308 || result < -1e308) {
            fprintf(stderr, "Error: Floating point overflow\n");
            return 1;
        }
    }
    
    printf("Alternating sum of first %d thousand numbers: %lld\n", n, sum);
    printf("Harmonic series sum for %d terms: %.15f\n", n, result);
    
    double pi_approx = 0.0;
    int sign = 1;
    for (int i = 0; i < 10000; i++) {
        double denominator = 2.0 * i + 1.0;
        if (denominator == 0.0) {
            continue;
        }
        pi_approx += sign * (4.0 / denominator);
        sign = -sign;
        
        if (pi_approx > 1e308 || pi_approx < -1e308) {
            break;
        }
    }
    
    printf("Pi approximation (Leibniz): %.15f\n", pi_approx);
    
    return 0;
}