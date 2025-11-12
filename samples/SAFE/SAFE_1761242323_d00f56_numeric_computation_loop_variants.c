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
        fprintf(stderr, "Error: Input out of range\n");
        return 1;
    }
    
    long long sum = 0;
    long long product = 1;
    double harmonic_sum = 0.0;
    
    printf("Computing using different loop variants:\n\n");
    
    printf("1. For loop (sum of squares): ");
    for (int i = 1; i <= n; i++) {
        if (i > 0 && sum > LLONG_MAX - (long long)i * i) {
            fprintf(stderr, "Error: Integer overflow\n");
            return 1;
        }
        sum += (long long)i * i;
    }
    printf("%lld\n", sum);
    
    printf("2. While loop (factorial): ");
    int temp = n;
    if (temp > 20) {
        printf("Too large for exact computation\n");
    } else {
        int j = 1;
        product = 1;
        while (j <= temp) {
            if (product > LLONG_MAX / j) {
                fprintf(stderr, "Error: Integer overflow\n");
                return 1;
            }
            product *= j;
            j++;
        }
        printf("%lld\n", product);
    }
    
    printf("3. Do-while loop (harmonic series): ");
    int k = 1;
    harmonic_sum = 0.0;
    do {
        harmonic_sum += 1.0 / k;
        k++;
    } while (k <= n);
    printf("%.6f\n", harmonic_sum);
    
    printf("4. Nested loops (prime count): ");
    int prime_count = 0;
    for (int num = 2; num <= n; num++) {
        int is_prime = 1;
        for (int divisor = 2; divisor * divisor <= num; divisor++) {
            if (num % divisor == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime) {
            prime_count++;
        }
    }
    printf("%d\n", prime_count);
    
    printf("5. Loop with continue (sum of odds): ");
    long long odd_sum = 0;
    for (int i = 1; i <= n; i++) {
        if (i % 2 == 0) {
            continue;
        }
        if (odd_sum > LLONG_MAX - i) {
            fprintf(stderr, "Error: Integer overflow\n");
            return 1;
        }
        odd_sum += i;
    }
    printf("%lld\n", odd_sum);
    
    return 0;
}