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
    for (uint64_t i = 2; i <= n; i++) {
        if (result > UINT64_MAX / i) return 0;
        result *= i;
    }
    return result;
}

double compute_pi_approximation(int iterations) {
    if (iterations <= 0 || iterations > MAX_ITERATIONS) return 0.0;
    
    double sum = 0.0;
    int sign = 1;
    
    for (int i = 0; i < iterations; i++) {
        double term = 1.0 / (2 * i + 1);
        sum += sign * term;
        sign = -sign;
    }
    
    return 4.0 * sum;
}

int main() {
    int choice;
    printf("Numeric Computation Menu:\n");
    printf("1. Check if number is prime\n");
    printf("2. Compute factorial\n");
    printf("3. Approximate pi using Leibniz formula\n");
    printf("Enter choice (1-3): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (choice < 1 || choice > 3) {
        printf("Invalid choice\n");
        return 1;
    }
    
    switch (choice) {
        case 1: {
            uint64_t num;
            printf("Enter number to check: ");
            if (scanf("%lu", &num) != 1) {
                printf("Invalid input\n");
                return 1;
            }
            printf("%lu is %sprime\n", num, is_prime(num) ? "" : "not ");
            break;
        }
        
        case 2: {
            uint64_t num;
            printf("Enter number for factorial (0-20): ");
            if (scanf("%lu", &num) != 1) {
                printf("Invalid input\n");
                return 1;
            }
            if (num > 20) {
                printf("Number too large for factorial\n");
                return 1;
            }
            uint64_t result = factorial(num);
            printf("%lu! = %lu\n", num, result);
            break;
        }
        
        case 3: {
            int iterations;
            printf("Enter number of iterations (1-%d): ", MAX_ITERATIONS);
            if (scanf("%d", &iterations) != 1) {
                printf("Invalid input\n");
                return 1;
            }
            if (iterations <= 0 || iterations > MAX_ITERATIONS) {
                printf("Invalid iteration count\n");
                return 1;
            }
            double pi_approx = compute_pi_approximation(iterations);
            printf("Pi approximation after %d iterations: %.15f\n", iterations, pi_approx);
            break;
        }
    }
    
    return 0;
}