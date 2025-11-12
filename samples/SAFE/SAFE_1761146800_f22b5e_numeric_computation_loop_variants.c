//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000

int main() {
    int n;
    printf("Enter number of terms to compute (1-%d): ", MAX_ITERATIONS);
    
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (n < 1 || n > MAX_ITERATIONS) {
        fprintf(stderr, "Error: Number must be between 1 and %d\n", MAX_ITERATIONS);
        return 1;
    }
    
    printf("Computing sequence using different loop variants:\n\n");
    
    printf("For loop (squares): ");
    for (int i = 1; i <= n; i++) {
        if (i > 1) printf(", ");
        long long result = (long long)i * i;
        printf("%lld", result);
    }
    printf("\n");
    
    printf("While loop (factorials): ");
    int j = 1;
    long long factorial = 1;
    while (j <= n) {
        if (j > 1) printf(", ");
        
        if (j > 20) {
            printf("OVERFLOW");
        } else {
            if (factorial > LLONG_MAX / j) {
                printf("OVERFLOW");
            } else {
                factorial *= j;
                printf("%lld", factorial);
            }
        }
        j++;
    }
    printf("\n");
    
    printf("Do-while loop (triangular numbers): ");
    int k = 1;
    long long triangular = 0;
    do {
        if (k > 1) printf(", ");
        
        if (triangular > LLONG_MAX - k) {
            printf("OVERFLOW");
        } else {
            triangular += k;
            printf("%lld", triangular);
        }
        k++;
    } while (k <= n);
    printf("\n");
    
    printf("Nested loops (multiplication table for %d):\n", (n > 10) ? 10 : n);
    int limit = (n > 10) ? 10 : n;
    for (int row = 1; row <= limit; row++) {
        for (int col = 1; col <= limit; col++) {
            printf("%4d", row * col);
        }
        printf("\n");
    }
    
    return 0;
}