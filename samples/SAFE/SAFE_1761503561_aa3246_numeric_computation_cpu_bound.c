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
            if (sum > LLONG_MAX - (long long)i * i) {
                fprintf(stderr, "Error: Overflow detected\n");
                return 1;
            }
            sum += (long long)i * i;
        } else {
            if (sum < LLONG_MIN + (long long)i * i) {
                fprintf(stderr, "Error: Underflow detected\n");
                return 1;
            }
            sum -= (long long)i * i;
        }
    }
    
    double result = (double)sum / (iterations > 0 ? iterations : 1);
    
    printf("Computed result: %.6f\n", result);
    printf("Iterations performed: %d\n", iterations);
    
    return 0;
}