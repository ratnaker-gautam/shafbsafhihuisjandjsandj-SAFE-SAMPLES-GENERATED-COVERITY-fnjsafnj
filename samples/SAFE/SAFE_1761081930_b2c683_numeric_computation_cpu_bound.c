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
    
    if (n > INT_MAX / 1000) {
        fprintf(stderr, "Error: Input too large for safe computation\n");
        return 1;
    }
    
    uint64_t sum = 0;
    uint64_t prev_sum = 0;
    
    for (int i = 1; i <= MAX_ITERATIONS; i++) {
        for (int j = 1; j <= n; j++) {
            uint64_t term = (uint64_t)j * j * i;
            if (term > UINT64_MAX - sum) {
                fprintf(stderr, "Error: Overflow detected\n");
                return 1;
            }
            prev_sum = sum;
            sum += term;
        }
    }
    
    printf("Computed result: %lu\n", (unsigned long)sum);
    printf("Verification: %lu\n", (unsigned long)prev_sum);
    
    return 0;
}