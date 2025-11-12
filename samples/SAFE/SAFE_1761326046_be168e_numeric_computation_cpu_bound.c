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
    
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= MAX_ITERATIONS; j++) {
            if (j > INT_MAX / i) {
                fprintf(stderr, "Error: Computation would overflow\n");
                return 1;
            }
            int term = i * j;
            if (term > INT_MAX / 1000) {
                fprintf(stderr, "Error: Computation would overflow\n");
                return 1;
            }
            int value = term % 1000;
            
            if (sum > UINT64_MAX - (uint64_t)value) {
                fprintf(stderr, "Error: Sum would overflow\n");
                return 1;
            }
            sum += (uint64_t)value;
        }
        
        if (sum < prev_sum) {
            fprintf(stderr, "Error: Sum overflow detected\n");
            return 1;
        }
        prev_sum = sum;
    }
    
    printf("Computed sum: %lu\n", (unsigned long)sum);
    printf("Average per iteration: %.2f\n", (double)sum / (n * MAX_ITERATIONS));
    
    return 0;
}