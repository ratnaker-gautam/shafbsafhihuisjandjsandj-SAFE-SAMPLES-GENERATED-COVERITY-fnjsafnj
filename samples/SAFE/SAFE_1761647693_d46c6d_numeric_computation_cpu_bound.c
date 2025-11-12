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
    uint64_t prev_sum = 0;
    
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            for (int k = 1; k <= n; k++) {
                uint64_t term = (uint64_t)i * j * k;
                if (term > UINT64_MAX - sum) {
                    fprintf(stderr, "Error: Integer overflow detected\n");
                    return 1;
                }
                prev_sum = sum;
                sum += term;
                if (sum < prev_sum) {
                    fprintf(stderr, "Error: Integer overflow detected\n");
                    return 1;
                }
            }
        }
    }
    
    uint64_t result = 0;
    for (int iter = 0; iter < MAX_ITERATIONS; iter++) {
        uint64_t temp = sum;
        for (int i = 0; i < 100; i++) {
            temp = (temp * 6364136223846793005ULL + 1) >> 1;
        }
        result ^= temp;
    }
    
    printf("Computation result: %lu\n", (unsigned long)result);
    return 0;
}