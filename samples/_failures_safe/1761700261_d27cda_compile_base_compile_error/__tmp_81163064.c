//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define MAX_ITERATIONS 1000
#define MAX_INPUT 1000000

int main() {
    int n;
    printf("Enter a positive integer (1-%d): ", MAX_INPUT);
    
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (n < 1 || n > MAX_INPUT) {
        fprintf(stderr, "Error: Number out of range\n");
        return 1;
    }
    
    printf("Computing mathematical properties for n = %d\n", n);
    
    long sum = 0;
    for (int i = 1; i <= n; i++) {
        if (sum > LONG_MAX - i) {
            fprintf(stderr, "Error: Integer overflow detected\n");
            return 1;
        }
        sum += i;
    }
    printf("Sum of first %d natural numbers: %ld\n", n, sum);
    
    long factorial = 1;
    int i = 1;
    while (i <= n) {
        if (i > 0 && factorial > LONG_MAX / i) {
            fprintf(stderr, "Error: Factorial would overflow\n");
            return 1;
        }
        factorial *= i;
        i++;
    }
    printf("Factorial of %d: %ld\n", n, factorial);
    
    double harmonic = 0.0;
    int j = 1;
    do {
        if (j != 0) {
            harmonic += 1.0 / j;
        }
        j++;
    } while (j <= n);
    printf("Harmonic number H_%d: %.6f\n", n, harmonic);
    
    int prime_count = 0;
    for (int num = 2; num <= n; num++) {
        int is_prime = 1;
        int limit = (int)sqrt(num);
        for (int k = 2; k <= limit; k++) {
            if (num % k == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime) {
            prime_count++;
        }
    }
    printf("Primes up to %d: %d\n", n, prime_count);
    
    double approx_pi = 0.0;
    int sign = 1;
    for (int term = 0; term < MAX_ITERATIONS; term++) {
        double denominator = 2 * term + 1;
        if (denominator == 0.0) {
            continue;
        }
        approx_pi += sign * (4.0 / denominator);
        sign = -sign;
    }
    printf("Pi approximation (Leibniz): %.8f\n", approx_pi);
    
    return 0;
}