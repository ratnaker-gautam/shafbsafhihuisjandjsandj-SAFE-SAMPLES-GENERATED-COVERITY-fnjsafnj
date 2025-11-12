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
    int input;
    printf("Enter a positive integer (1-1000): ");
    
    if (scanf("%d", &input) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (input < 1 || input > 1000) {
        fprintf(stderr, "Error: Input out of range\n");
        return 1;
    }
    
    uint64_t sum_primes = 0;
    uint64_t prime_count = 0;
    uint64_t current = 2;
    
    while (prime_count < MAX_ITERATIONS && prime_count < (uint64_t)input) {
        if (is_prime(current)) {
            sum_primes += current;
            prime_count++;
        }
        current++;
    }
    
    uint64_t fact_result = factorial((uint64_t)input % 21);
    
    printf("Sum of first %d primes: %lu\n", input, sum_primes);
    printf("Factorial of %d mod 21: %lu\n", input % 21, fact_result);
    
    double pi_approx = 0.0;
    int terms = input * 100;
    if (terms > MAX_ITERATIONS) terms = MAX_ITERATIONS;
    
    for (int i = 0; i < terms; i++) {
        double term = 1.0 / (2 * i + 1);
        if (i % 2 == 0) {
            pi_approx += term;
        } else {
            pi_approx -= term;
        }
    }
    pi_approx *= 4.0;
    
    printf("Pi approximation (%d terms): %.10f\n", terms, pi_approx);
    
    return 0;
}