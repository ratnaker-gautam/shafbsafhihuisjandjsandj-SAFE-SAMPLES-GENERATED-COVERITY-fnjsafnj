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
    int input;
    printf("Enter a positive integer (1-100): ");
    
    if (scanf("%d", &input) != 1) {
        printf("Invalid input: not a number\n");
        return 1;
    }
    
    if (input < 1 || input > 100) {
        printf("Invalid input: must be between 1 and 100\n");
        return 1;
    }
    
    uint64_t sum_primes = 0;
    uint64_t prime_count = 0;
    uint64_t current = 2;
    
    while (prime_count < MAX_ITERATIONS && prime_count < (uint64_t)input * 100) {
        if (is_prime(current)) {
            sum_primes += current;
            prime_count++;
        }
        current++;
    }
    
    uint64_t fact_result = factorial((uint64_t)input);
    
    printf("Sum of first %lu primes: %lu\n", prime_count, sum_primes);
    if (fact_result > 0) {
        printf("Factorial of %d: %lu\n", input, fact_result);
    } else {
        printf("Factorial of %d: overflow\n", input);
    }
    
    double pi_approx = 0.0;
    int sign = 1;
    for (int i = 0; i < 100000; i++) {
        double term = sign / (2.0 * i + 1.0);
        pi_approx += term;
        sign = -sign;
    }
    pi_approx *= 4.0;
    
    printf("Pi approximation: %.10f\n", pi_approx);
    
    return 0;
}