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
    
    uint64_t sum = 0;
    uint64_t max_safe = UINT64_MAX / (uint64_t)(n * 2);
    
    if (MAX_ITERATIONS > max_safe) {
        fprintf(stderr, "Error: Computation would overflow\n");
        return 1;
    }
    
    for (int i = 1; i <= MAX_ITERATIONS; i++) {
        if (i > max_safe) {
            fprintf(stderr, "Error: Computation overflow detected\n");
            return 1;
        }
        
        uint64_t term = (uint64_t)i * (uint64_t)n;
        if (UINT64_MAX - sum < term) {
            fprintf(stderr, "Error: Sum overflow detected\n");
            return 1;
        }
        sum += term;
    }
    
    double average = (double)sum / (double)MAX_ITERATIONS;
    
    printf("Computed average after %d iterations: %.6f\n", MAX_ITERATIONS, average);
    printf("Sum: %lu\n", sum);
    
    return 0;
}