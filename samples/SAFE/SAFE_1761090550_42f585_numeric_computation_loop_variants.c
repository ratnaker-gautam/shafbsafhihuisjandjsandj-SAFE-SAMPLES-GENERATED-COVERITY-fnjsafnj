//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
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
        if (sum > LLONG_MAX - i) {
            fprintf(stderr, "Error: Integer overflow in sum\n");
            return 1;
        }
        sum += i;
        
        if (product > LLONG_MAX / i) {
            fprintf(stderr, "Error: Integer overflow in product\n");
            return 1;
        }
        product *= i;
        
        long long square = (long long)i * i;
        if (square < 0 || square > LLONG_MAX - squares_sum) {
            fprintf(stderr, "Error: Integer overflow in squares sum\n");
            return 1;
        }
        squares_sum += square;
        
        i++;
    }
    
    printf("Results using while loop:\n");
    printf("Sum: %lld\n", sum);
    printf("Product: %lld\n", product);
    printf("Sum of squares: %lld\n", squares_sum);
    
    printf("\nComputing factorial sequence using for loop:\n");
    for (int j = 1; j <= n; j++) {
        long long fact = 1;
        for (int k = 1; k <= j; k++) {
            if (fact > LLONG_MAX / k) {
                fprintf(stderr, "Error: Integer overflow in factorial\n");
                return 1;
            }
            fact *= k;
        }
        printf("Factorial(%d) = %lld\n", j, fact);
    }
    
    printf("\nComputing alternating sum using do-while loop:\n");
    int m = n;
    long long alt_sum = 0;
    int sign = 1;
    
    do {
        if (sign > 0) {
            if (alt_sum > LLONG_MAX - m) {
                fprintf(stderr, "Error: Integer overflow in alternating sum\n");
                return 1;
            }
            alt_sum += m;
        } else {
            if (alt_sum < LLONG_MIN + m) {
                fprintf(stderr, "Error: Integer overflow in alternating sum\n");
                return 1;
            }
            alt_sum -= m;
        }
        sign = -sign;
        m--;
    } while (m > 0);
    
    printf("Alternating sum: %lld\n", alt_sum);
    
    return 0;
}