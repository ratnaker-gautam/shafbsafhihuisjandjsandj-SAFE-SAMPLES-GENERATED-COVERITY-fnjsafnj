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
    
    for (int iter = 0; iter < MAX_ITERATIONS; iter++) {
        sum = 0;
        
        for (int i = 1; i <= n; i++) {
            long long term = (long long)i * i;
            
            if (term > LLONG_MAX - sum) {
                fprintf(stderr, "Error: Integer overflow detected\n");
                return 1;
            }
            
            sum += term;
        }
        
        if (iter > 0 && sum != prev_sum) {
            fprintf(stderr, "Error: Non-deterministic computation\n");
            return 1;
        }
        
        prev_sum = sum;
    }
    
    printf("Sum of squares from 1 to %d computed %d times: %lld\n", n, MAX_ITERATIONS, sum);
    
    double average = (double)sum / n;
    printf("Average of squares: %.2f\n", average);
    
    long long product = 1;
    for (int i = 1; i <= n; i++) {
        if (i > 0 && product > LLONG_MAX / i) {
            fprintf(stderr, "Error: Integer overflow in product\n");
            return 1;
        }
        product *= i;
    }
    
    printf("Factorial of %d: %lld\n", n, product);
    
    return 0;
}