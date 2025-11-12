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
    
    long long sum = 0;
    long long prev_sum = 0;
    
    for (int i = 1; i <= MAX_ITERATIONS; i++) {
        for (int j = 1; j <= n; j++) {
            long long term = (long long)j * j * i;
            
            if (term > LLONG_MAX - sum) {
                fprintf(stderr, "Error: Integer overflow detected\n");
                return 1;
            }
            
            sum += term;
        }
        
        if (i % 100000 == 0) {
            printf("Iteration %d: Current sum = %lld\n", i, sum);
        }
        
        if (sum == prev_sum && i > n * 10) {
            printf("Converged at iteration %d\n", i);
            break;
        }
        
        prev_sum = sum;
    }
    
    printf("Final result: %lld\n", sum);
    
    double average = (double)sum / (double)MAX_ITERATIONS;
    printf("Average per iteration: %.2f\n", average);
    
    return 0;
}