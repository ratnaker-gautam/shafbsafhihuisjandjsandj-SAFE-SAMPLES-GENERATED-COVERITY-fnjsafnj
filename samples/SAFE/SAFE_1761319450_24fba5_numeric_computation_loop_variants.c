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
        
        long long square = (long long)i * i;
        if (square / i != i) {
            fprintf(stderr, "Error: Integer overflow in square\n");
            return 1;
        }
        
        if (squares_sum > LLONG_MAX - square) {
            fprintf(stderr, "Error: Integer overflow in squares sum\n");
            return 1;
        }
        squares_sum += square;
        
        i++;
    }
    
    printf("Results:\n");
    printf("Sum: %lld\n", sum);
    printf("Product: %lld\n", product);
    printf("Sum of squares: %lld\n", squares_sum);
    
    double harmonic_mean = 0.0;
    for (int j = 1; j <= n; j++) {
        harmonic_mean += 1.0 / j;
    }
    harmonic_mean = n / harmonic_mean;
    
    printf("Harmonic mean: %.6f\n", harmonic_mean);
    
    long long factorial = 1;
    int k = 1;
    do {
        if (k > 0 && factorial > LLONG_MAX / k) {
            fprintf(stderr, "Error: Integer overflow in factorial\n");
            return 1;
        }
        factorial *= k;
        k++;
    } while (k <= n);
    
    printf("Factorial: %lld\n", factorial);
    
    return 0;
}