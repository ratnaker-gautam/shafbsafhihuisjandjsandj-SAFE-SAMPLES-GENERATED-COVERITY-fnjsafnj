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
    int iterations = (n < MAX_ITERATIONS) ? n : MAX_ITERATIONS;
    
    for (int i = 1; i <= iterations; i++) {
        if (i > INT_MAX / i) {
            fprintf(stderr, "Error: Computation would overflow\n");
            return 1;
        }
        
        int square = i * i;
        if (square > INT_MAX - sum) {
            fprintf(stderr, "Error: Sum would overflow\n");
            return 1;
        }
        
        sum += square;
        
        for (int j = 1; j <= i; j++) {
            if (j > INT_MAX / j / j) {
                fprintf(stderr, "Error: Computation would overflow\n");
                return 1;
            }
            
            int cube = j * j * j;
            if (cube > INT_MAX - sum) {
                fprintf(stderr, "Error: Sum would overflow\n");
                return 1;
            }
            
            sum += cube;
        }
    }
    
    printf("Computed sum: %lld\n", sum);
    printf("Iterations completed: %d\n", iterations);
    
    return 0;
}