//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000000

int is_prime(uint64_t n) {
    if (n < 2) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    
    for (uint64_t i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return 0;
    }
    return 1;
}

uint64_t factorial(uint64_t n) {
    if (n > 20) return 0;
    uint64_t result = 1;
    for (uint64_t i = 1; i <= n; i++) {
        if (result > UINT64_MAX / i) return 0;
        result *= i;
    }
    return result;
}

int main() {
    int iterations;
    printf("Enter number of iterations (1-%d): ", MAX_ITERATIONS);
    
    if (scanf("%d", &iterations) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (iterations < 1 || iterations > MAX_ITERATIONS) {
        fprintf(stderr, "Error: Iterations must be between 1 and %d\n", MAX_ITERATIONS);
        return 1;
    }
    
    uint64_t prime_sum = 0;
    uint64_t factorial_sum = 0;
    uint64_t numbers_processed = 0;
    
    for (int i = 0; i < iterations; i++) {
        uint64_t current_num = (uint64_t)i + 1;
        
        if (is_prime(current_num)) {
            if (prime_sum > UINT64_MAX - current_num) {
                fprintf(stderr, "Error: Integer overflow detected\n");
                return 1;
            }
            prime_sum += current_num;
        }
        
        uint64_t fact = factorial(current_num % 10);
        if (factorial_sum > UINT64_MAX - fact) {
            fprintf(stderr, "Error: Integer overflow detected\n");
            return 1;
        }
        factorial_sum += fact;
        
        numbers_processed++;
    }
    
    printf("Numbers processed: %lu\n", numbers_processed);
    printf("Sum of primes: %lu\n", prime_sum);
    printf("Sum of factorials: %lu\n", factorial_sum);
    
    return 0;
}