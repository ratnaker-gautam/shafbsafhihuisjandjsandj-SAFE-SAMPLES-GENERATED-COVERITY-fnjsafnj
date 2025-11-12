//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
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
    int count_even = 0;
    int count_odd = 0;
    
    printf("Computing sum, product, and parity counts for numbers 1 to %d...\n", n);
    
    int i = 1;
    while (i <= n) {
        if (i > 0 && sum > LLONG_MAX - i) {
            fprintf(stderr, "Error: Integer overflow in sum\n");
            return 1;
        }
        sum += i;
        
        if (i > 0 && product > LLONG_MAX / i) {
            fprintf(stderr, "Error: Integer overflow in product\n");
            return 1;
        }
        product *= i;
        
        if (i % 2 == 0) {
            count_even++;
        } else {
            count_odd++;
        }
        i++;
    }
    
    printf("Results:\n");
    printf("Sum (1 to %d): %lld\n", n, sum);
    printf("Product (1 to %d): %lld\n", n, product);
    printf("Even numbers: %d\n", count_even);
    printf("Odd numbers: %d\n", count_odd);
    
    double average = (double)sum / n;
    printf("Average: %.2f\n", average);
    
    int j;
    long long sum_squares = 0;
    for (j = 1; j <= n; j++) {
        if (j > 0 && (long long)j * j > LLONG_MAX - sum_squares) {
            fprintf(stderr, "Error: Integer overflow in sum of squares\n");
            return 1;
        }
        sum_squares += (long long)j * j;
    }
    printf("Sum of squares: %lld\n", sum_squares);
    
    int k = 1;
    long long sum_cubes = 0;
    do {
        if (k > 0 && (long long)k * k * k > LLONG_MAX - sum_cubes) {
            fprintf(stderr, "Error: Integer overflow in sum of cubes\n");
            return 1;
        }
        sum_cubes += (long long)k * k * k;
        k++;
    } while (k <= n);
    printf("Sum of cubes: %lld\n", sum_cubes);
    
    return 0;
}