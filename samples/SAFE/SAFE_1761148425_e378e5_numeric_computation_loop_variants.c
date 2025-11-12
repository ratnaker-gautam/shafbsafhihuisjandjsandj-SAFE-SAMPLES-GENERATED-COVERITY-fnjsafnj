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
    
    printf("Computing alternating sum using different loop:\n");
    long long alt_sum = 0;
    int sign = 1;
    for (int m = 1; m <= n; m++) {
        long long term = (long long)m * sign;
        if ((sign > 0 && alt_sum > LLONG_MAX - term) || 
            (sign < 0 && alt_sum < LLONG_MIN - term)) {
            fprintf(stderr, "Error: Integer overflow in alternating sum\n");
            return 1;
        }
        alt_sum += term;
        sign = -sign;
    }
    printf("Alternating sum: %lld\n", alt_sum);
    
    return 0;
}