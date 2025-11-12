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
    
    uint64_t sum = 0;
    uint64_t prev_sum = 0;
    
    for (int i = 1; i <= MAX_ITERATIONS; i++) {
        for (int j = 1; j <= n; j++) {
            if (j > 0 && i > 0) {
                uint64_t term = (uint64_t)j * (uint64_t)i;
                if (term > UINT64_MAX - sum) {
                    fprintf(stderr, "Error: Arithmetic overflow detected\n");
                    return 1;
                }
                sum += term;
            }
        }
        
        if (i % 100000 == 0) {
            printf("Progress: iteration %d, current sum: %lu\n", i, sum);
        }
        
        if (i > 1 && sum == prev_sum) {
            printf("Converged at iteration %d\n", i);
            break;
        }
        
        prev_sum = sum;
        
        if (i == MAX_ITERATIONS) {
            printf("Reached maximum iterations\n");
        }
    }
    
    printf("Final result: %lu\n", sum);
    
    return 0;
}