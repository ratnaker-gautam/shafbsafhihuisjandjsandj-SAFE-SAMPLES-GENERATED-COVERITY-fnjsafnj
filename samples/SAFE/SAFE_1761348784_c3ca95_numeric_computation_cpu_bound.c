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
    
    if (n > INT_MAX / 2) {
        fprintf(stderr, "Error: Input too large for safe computation\n");
        return 1;
    }
    
    uint64_t sum = 0;
    uint64_t prev_sum = 0;
    
    for (int i = 1; i <= n; i++) {
        if (i > 0 && sum > UINT64_MAX - (uint64_t)i * i) {
            fprintf(stderr, "Error: Overflow detected\n");
            return 1;
        }
        prev_sum = sum;
        sum += (uint64_t)i * i;
    }
    
    uint64_t result = 0;
    for (int iter = 0; iter < MAX_ITERATIONS; iter++) {
        if (iter > 0 && result > UINT64_MAX - sum) {
            fprintf(stderr, "Error: Overflow in iteration\n");
            return 1;
        }
        result += sum;
    }
    
    printf("Computed result: %lu\n", (unsigned long)result);
    printf("Sum of squares for n=%d: %lu\n", n, (unsigned long)sum);
    
    return 0;
}