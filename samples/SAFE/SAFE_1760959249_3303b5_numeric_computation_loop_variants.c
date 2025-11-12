//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000
#define MAX_INPUT 1000000

int main(void) {
    int n;
    printf("Enter a positive integer (1-%d): ", MAX_INPUT);
    
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (n <= 0 || n > MAX_INPUT) {
        fprintf(stderr, "Error: Number out of range\n");
        return 1;
    }
    
    long long sum = 0;
    long long product = 1;
    int count = 0;
    
    printf("Computing sum and product of first %d natural numbers...\n", n);
    
    int i = 1;
    while (i <= n) {
        if (product > LLONG_MAX / i) {
            fprintf(stderr, "Error: Integer overflow detected\n");
            return 1;
        }
        sum += i;
        product *= i;
        i++;
    }
    
    printf("Sum: %lld\n", sum);
    printf("Product: %lld\n", product);
    
    double harmonic_mean = 0.0;
    int j = 1;
    do {
        harmonic_mean += 1.0 / j;
        j++;
    } while (j <= n);
    
    if (harmonic_mean > 0.0) {
        harmonic_mean = (double)n / harmonic_mean;
        printf("Harmonic mean: %.6f\n", harmonic_mean);
    }
    
    long long sum_squares = 0;
    for (int k = 1; k <= n; k++) {
        if (sum_squares > LLONG_MAX - (long long)k * k) {
            fprintf(stderr, "Error: Integer overflow detected\n");
            return 1;
        }
        sum_squares += (long long)k * k;
    }
    printf("Sum of squares: %lld\n", sum_squares);
    
    int primes_found = 0;
    printf("First %d prime numbers: ", n > 10 ? 10 : n);
    
    for (int num = 2; primes_found < (n > 10 ? 10 : n); num++) {
        int is_prime = 1;
        
        for (int div = 2; div * div <= num; div++) {
            if (num % div == 0) {
                is_prime = 0;
                break;
            }
        }
        
        if (is_prime) {
            printf("%d ", num);
            primes_found++;
        }
        
        if (primes_found >= MAX_ITERATIONS) {
            break;
        }
    }
    printf("\n");
    
    return 0;
}