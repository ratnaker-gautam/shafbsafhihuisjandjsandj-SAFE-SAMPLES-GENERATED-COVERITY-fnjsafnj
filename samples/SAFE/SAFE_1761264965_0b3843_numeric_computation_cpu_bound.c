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
    int choice;
    printf("Select operation:\n1. Find primes up to N\n2. Calculate factorial\n");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (choice == 1) {
        uint64_t limit;
        printf("Enter upper limit (2-1000000): ");
        if (scanf("%lu", &limit) != 1) {
            printf("Invalid input\n");
            return 1;
        }
        
        if (limit < 2 || limit > 1000000) {
            printf("Limit out of range\n");
            return 1;
        }
        
        uint64_t count = 0;
        for (uint64_t i = 2; i <= limit; i++) {
            if (is_prime(i)) {
                count++;
            }
        }
        
        printf("Found %lu primes up to %lu\n", count, limit);
        
    } else if (choice == 2) {
        uint64_t n;
        printf("Enter number (0-20): ");
        if (scanf("%lu", &n) != 1) {
            printf("Invalid input\n");
            return 1;
        }
        
        if (n > 20) {
            printf("Number too large for factorial calculation\n");
            return 1;
        }
        
        uint64_t result = factorial(n);
        if (result == 0 && n > 0) {
            printf("Factorial overflow\n");
            return 1;
        }
        
        printf("%lu! = %lu\n", n, result);
        
    } else {
        printf("Invalid choice\n");
        return 1;
    }
    
    return 0;
}