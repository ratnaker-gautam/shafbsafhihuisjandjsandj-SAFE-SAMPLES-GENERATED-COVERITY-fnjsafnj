//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define MAX_ITERATIONS 1000000
#define MAX_INPUT 1000000

int is_prime(uint64_t n) {
    if (n <= 1) return 0;
    if (n <= 3) return 1;
    if (n % 2 == 0 || n % 3 == 0) return 0;
    for (uint64_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return 0;
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

double compute_pi_approximation(int iterations) {
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
    printf("Select computation:\n");
    printf("1. Prime number check\n");
    printf("2. Factorial calculation\n");
    printf("3. Pi approximation\n");
    
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
            printf("Enter number to check (1-%u): ", MAX_INPUT);
            if (scanf("%lu", &num) != 1) {
                printf("Invalid input\n");
                return 1;
            }
            if (num > MAX_INPUT) {
                printf("Number too large\n");
                return 1;
            }
            if (is_prime(num)) {
                printf("%lu is prime\n", num);
            } else {
                printf("%lu is not prime\n", num);
            }
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
            if (result == 0 && num > 0) {
                printf("Factorial overflow\n");
                return 1;
            }
            printf("%lu! = %lu\n", num, result);
            break;
        }
        case 3: {
            int iterations;
            printf("Enter iterations for pi approximation (1-%d): ", MAX_ITERATIONS);
            if (scanf("%d", &iterations) != 1) {
                printf("Invalid input\n");
                return 1;
            }
            if (iterations < 1 || iterations > MAX_ITERATIONS) {
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