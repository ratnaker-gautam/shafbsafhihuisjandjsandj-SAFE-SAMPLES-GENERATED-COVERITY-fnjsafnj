//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <math.h>

#define MAX_ITERATIONS 1000000
#define MAX_INPUT 100000

int is_prime(uint64_t n) {
    if (n < 2) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    
    uint64_t limit = (uint64_t)sqrt((double)n) + 1;
    for (uint64_t i = 3; i <= limit; i += 2) {
        if (n % i == 0) return 0;
    }
    return 1;
}

uint64_t factorial(uint64_t n) {
    if (n > 20) return UINT64_MAX;
    uint64_t result = 1;
    for (uint64_t i = 2; i <= n; i++) {
        if (result > UINT64_MAX / i) return UINT64_MAX;
        result *= i;
    }
    return result;
}

double compute_pi_approximation(int iterations) {
    if (iterations <= 0 || iterations > MAX_ITERATIONS) return 0.0;
    
    double sum = 0.0;
    for (int i = 0; i < iterations; i++) {
        double term = 1.0 / (2.0 * i + 1.0);
        if (i % 2 == 0) {
            sum += term;
        } else {
            sum -= term;
        }
    }
    return 4.0 * sum;
}

int main() {
    int choice;
    printf("Numeric Computation Menu:\n");
    printf("1. Prime number check\n");
    printf("2. Factorial calculation\n");
    printf("3. Pi approximation\n");
    printf("Enter choice (1-3): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    switch (choice) {
        case 1: {
            uint64_t num;
            printf("Enter number to check (2-%llu): ", (unsigned long long)MAX_INPUT);
            if (scanf("%llu", (unsigned long long*)&num) != 1) {
                printf("Invalid input\n");
                return 1;
            }
            if (num < 2 || num > MAX_INPUT) {
                printf("Number out of range\n");
                return 1;
            }
            if (is_prime(num)) {
                printf("%llu is prime\n", (unsigned long long)num);
            } else {
                printf("%llu is not prime\n", (unsigned long long)num);
            }
            break;
        }
        case 2: {
            uint64_t num;
            printf("Enter number for factorial (0-20): ");
            if (scanf("%llu", (unsigned long long*)&num) != 1) {
                printf("Invalid input\n");
                return 1;
            }
            if (num > 20) {
                printf("Number too large for factorial\n");
                return 1;
            }
            uint64_t result = factorial(num);
            if (result == UINT64_MAX) {
                printf("Factorial overflow\n");
            } else {
                printf("%llu! = %llu\n", (unsigned long long)num, (unsigned long long)result);
            }
            break;
        }
        case 3: {
            int iterations;
            printf("Enter iterations for Pi approximation (1-%d): ", MAX_ITERATIONS);
            if (scanf("%d", &iterations) != 1) {
                printf("Invalid input\n");
                return 1;
            }
            if (iterations <= 0 || iterations > MAX_ITERATIONS) {
                printf("Iterations out of range\n");
                return 1;
            }
            double pi_approx = compute_pi_approximation(iterations);
            printf("Pi approximation after %d iterations: %.15f\n", iterations, pi_approx);
            break;
        }
        default:
            printf("Invalid choice\n");
            return 1;
    }
    
    return 0;
}