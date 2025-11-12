//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000000

int is_prime(uint64_t n) {
    if (n <= 1) return 0;
    if (n <= 3) return 1;
    if (n % 2 == 0 || n % 3 == 0) return 0;
    
    for (uint64_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) {
            return 0;
        }
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
    int start, end;
    char input[256];
    
    printf("Enter start number (1-1000): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    if (sscanf(input, "%d", &start) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    printf("Enter end number (1-1000): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    if (sscanf(input, "%d", &end) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (start < 1 || end < 1 || start > 1000 || end > 1000) {
        fprintf(stderr, "Numbers must be between 1 and 1000\n");
        return 1;
    }
    
    if (start > end) {
        int temp = start;
        start = end;
        end = temp;
    }
    
    uint64_t prime_sum = 0;
    uint64_t factorial_sum = 0;
    int prime_count = 0;
    
    for (int i = start; i <= end; i++) {
        if (is_prime((uint64_t)i)) {
            prime_sum += (uint64_t)i;
            prime_count++;
        }
        
        uint64_t fact = factorial((uint64_t)i);
        if (fact > 0) {
            if (factorial_sum > UINT64_MAX - fact) {
                fprintf(stderr, "Factorial sum overflow\n");
                return 1;
            }
            factorial_sum += fact;
        }
    }
    
    printf("Range: %d to %d\n", start, end);
    printf("Primes found: %d\n", prime_count);
    printf("Sum of primes: %lu\n", prime_sum);
    printf("Sum of factorials: %lu\n", factorial_sum);
    
    double pi_approx = 0.0;
    int sign = 1;
    for (int i = 0; i < MAX_ITERATIONS; i++) {
        double term = sign / (2.0 * i + 1.0);
        pi_approx += term;
        sign = -sign;
    }
    pi_approx *= 4.0;
    
    printf("Pi approximation (Leibniz): %.10f\n", pi_approx);
    
    return 0;
}