//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: numeric_computation
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
    int input_count;
    printf("Enter number of computations (1-100): ");
    
    if (scanf("%d", &input_count) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (input_count < 1 || input_count > 100) {
        fprintf(stderr, "Error: Count must be between 1 and 100\n");
        return 1;
    }
    
    uint64_t numbers[100];
    printf("Enter %d numbers between 1 and 1000000:\n", input_count);
    
    for (int i = 0; i < input_count; i++) {
        if (scanf("%lu", &numbers[i]) != 1) {
            fprintf(stderr, "Error: Invalid number input\n");
            return 1;
        }
        
        if (numbers[i] < 1 || numbers[i] > 1000000) {
            fprintf(stderr, "Error: Number must be between 1 and 1000000\n");
            return 1;
        }
    }
    
    printf("\nResults:\n");
    printf("Number\tPrime\tFactorial\tPrimeCount\n");
    
    for (int i = 0; i < input_count; i++) {
        uint64_t n = numbers[i];
        int prime_result = is_prime(n);
        uint64_t fact_result = factorial(n);
        uint64_t prime_count = 0;
        
        for (uint64_t j = 2; j <= n && j <= MAX_ITERATIONS; j++) {
            if (is_prime(j)) {
                prime_count++;
            }
        }
        
        printf("%lu\t%s\t", n, prime_result ? "Yes" : "No");
        
        if (fact_result == 0 && n > 0) {
            printf("Overflow\t");
        } else {
            printf("%lu\t", fact_result);
        }
        
        printf("%lu\n", prime_count);
    }
    
    uint64_t total_primes = 0;
    for (uint64_t i = 2; i <= 100000 && i <= MAX_ITERATIONS; i++) {
        if (is_prime(i)) {
            total_primes++;
        }
    }
    
    printf("\nTotal primes up to 100000: %lu\n", total_primes);
    
    return 0;
}