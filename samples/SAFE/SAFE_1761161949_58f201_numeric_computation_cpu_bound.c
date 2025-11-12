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
    long long prev_sum = 0;
    
    for (int i = 1; i <= MAX_ITERATIONS; i++) {
        prev_sum = sum;
        
        for (int j = 1; j <= n; j++) {
            if (j > 0 && sum > LLONG_MAX - (long long)j * j) {
                fprintf(stderr, "Error: Integer overflow detected\n");
                return 1;
            }
            sum += (long long)j * j;
        }
        
        if (i % 100000 == 0) {
            printf("Iteration %d: Current sum = %lld\n", i, sum);
        }
        
        if (sum < prev_sum) {
            fprintf(stderr, "Error: Integer overflow detected\n");
            return 1;
        }
    }
    
    printf("Final result after %d iterations: %lld\n", MAX_ITERATIONS, sum);
    
    return 0;
}