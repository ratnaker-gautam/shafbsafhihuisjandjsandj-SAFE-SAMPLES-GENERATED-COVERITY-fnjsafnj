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
    double harmonic_sum = 0.0;
    
    printf("Computing series up to %d terms:\n", n);
    
    int i = 1;
    while (i <= n) {
        long long term = (long long)i * i;
        if (term > LLONG_MAX - sum) {
            fprintf(stderr, "Error: Sum overflow\n");
            return 1;
        }
        sum += term;
        
        if (product > LLONG_MAX / i) {
            fprintf(stderr, "Error: Product overflow\n");
            return 1;
        }
        product *= i;
        
        harmonic_sum += 1.0 / i;
        i++;
    }
    
    printf("Sum of squares: %lld\n", sum);
    printf("Factorial: %lld\n", product);
    printf("Harmonic sum: %.6f\n", harmonic_sum);
    
    double geometric_mean = 1.0;
    for (int j = 1; j <= n; j++) {
        geometric_mean *= j;
    }
    geometric_mean = pow(geometric_mean, 1.0 / n);
    printf("Geometric mean: %.6f\n", geometric_mean);
    
    long long fibonacci[3] = {0, 1, 1};
    printf("Fibonacci sequence: ");
    for (int k = 0; k < n && k < 3; k++) {
        printf("%lld ", fibonacci[k]);
    }
    
    int count = 3;
    do {
        if (count >= n) break;
        long long next = fibonacci[1] + fibonacci[2];
        if (next < fibonacci[2]) {
            fprintf(stderr, "Error: Fibonacci overflow\n");
            return 1;
        }
        fibonacci[0] = fibonacci[1];
        fibonacci[1] = fibonacci[2];
        fibonacci[2] = next;
        printf("%lld ", next);
        count++;
    } while (count < n);
    
    printf("\n");
    
    return 0;
}