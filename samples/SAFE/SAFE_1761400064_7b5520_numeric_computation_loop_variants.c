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
        if (product > LLONG_MAX / i) {
            fprintf(stderr, "Error: Product overflow\n");
            return 1;
        }
        product *= i;
        if (i <= INT_MAX / i) {
            squares_sum += (long long)i * i;
        } else {
            fprintf(stderr, "Error: Square overflow\n");
            return 1;
        }
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
    
    long long fibonacci[101] = {0, 1};
    if (n >= 1) {
        for (int k = 2; k <= n && k < 101; k++) {
            if (fibonacci[k-1] > LLONG_MAX - fibonacci[k-2]) {
                fprintf(stderr, "Error: Fibonacci overflow\n");
                return 1;
            }
            fibonacci[k] = fibonacci[k-1] + fibonacci[k-2];
        }
        
        printf("First %d Fibonacci numbers:\n", n);
        int idx = 0;
        do {
            printf("%lld ", fibonacci[idx]);
            idx++;
        } while (idx < n && idx < 101);
        printf("\n");
    }
    
    return 0;
}