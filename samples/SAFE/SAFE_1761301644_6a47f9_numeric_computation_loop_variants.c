//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

int main() {
    int n;
    printf("Enter number of terms to compute (1-100): ");
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (n < 1 || n > 100) {
        fprintf(stderr, "Error: Number must be between 1 and 100\n");
        return 1;
    }
    
    long long sum = 0;
    long long product = 1;
    long long squares_sum = 0;
    
    printf("Computing sum, product, and sum of squares for first %d natural numbers:\n", n);
    
    int i = 1;
    while (i <= n) {
        sum += i;
        if (sum < 0) {
            fprintf(stderr, "Error: Integer overflow in sum\n");
            return 1;
        }
        i++;
    }
    
    for (int j = 1; j <= n; j++) {
        if (product > LLONG_MAX / j) {
            fprintf(stderr, "Error: Integer overflow in product\n");
            return 1;
        }
        product *= j;
    }
    
    int k = 1;
    do {
        long long square = (long long)k * k;
        if (square < 0 || squares_sum > LLONG_MAX - square) {
            fprintf(stderr, "Error: Integer overflow in squares sum\n");
            return 1;
        }
        squares_sum += square;
        k++;
    } while (k <= n);
    
    printf("Sum: %lld\n", sum);
    printf("Product: %lld\n", product);
    printf("Sum of squares: %lld\n", squares_sum);
    
    printf("\nComputing alternating sum using different loop:\n");
    long long alt_sum = 0;
    for (int m = 1; m <= n; m++) {
        if (m % 2 == 0) {
            if (alt_sum < LLONG_MIN + m) {
                fprintf(stderr, "Error: Integer overflow in alternating sum\n");
                return 1;
            }
            alt_sum -= m;
        } else {
            if (alt_sum > LLONG_MAX - m) {
                fprintf(stderr, "Error: Integer overflow in alternating sum\n");
                return 1;
            }
            alt_sum += m;
        }
    }
    printf("Alternating sum: %lld\n", alt_sum);
    
    printf("\nComputing factorial sequence:\n");
    for (int p = 1; p <= n && p <= 20; p++) {
        long long fact = 1;
        for (int q = 1; q <= p; q++) {
            if (fact > LLONG_MAX / q) {
                fprintf(stderr, "Error: Factorial too large for p=%d\n", p);
                return 1;
            }
            fact *= q;
        }
        printf("Factorial(%d) = %lld\n", p, fact);
    }
    
    return 0;
}