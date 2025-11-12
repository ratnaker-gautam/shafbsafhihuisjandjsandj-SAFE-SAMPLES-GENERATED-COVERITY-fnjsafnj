//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

#define MAX_INPUT 100

int main() {
    char input[MAX_INPUT + 1];
    int n;
    
    printf("Enter a positive integer (1-100): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (sscanf(input, "%d", &n) != 1) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    if (n < 1 || n > 100) {
        fprintf(stderr, "Number out of range\n");
        return 1;
    }
    
    printf("Computing sum of squares using different loop variants:\n\n");
    
    printf("1. For loop: ");
    long long sum_for = 0;
    for (int i = 1; i <= n; i++) {
        if (i > 0 && sum_for > LLONG_MAX - (long long)i * i) {
            fprintf(stderr, "Integer overflow detected\n");
            return 1;
        }
        sum_for += (long long)i * i;
    }
    printf("%lld\n", sum_for);
    
    printf("2. While loop: ");
    long long sum_while = 0;
    int j = 1;
    while (j <= n) {
        if (j > 0 && sum_while > LLONG_MAX - (long long)j * j) {
            fprintf(stderr, "Integer overflow detected\n");
            return 1;
        }
        sum_while += (long long)j * j;
        j++;
    }
    printf("%lld\n", sum_while);
    
    printf("3. Do-while loop: ");
    long long sum_do = 0;
    int k = 1;
    do {
        if (k > 0 && sum_do > LLONG_MAX - (long long)k * k) {
            fprintf(stderr, "Integer overflow detected\n");
            return 1;
        }
        sum_do += (long long)k * k;
        k++;
    } while (k <= n);
    printf("%lld\n", sum_do);
    
    printf("4. Nested loops (matrix sum): ");
    long long sum_nested = 0;
    for (int row = 1; row <= n; row++) {
        for (int col = 1; col <= n; col++) {
            long long product = (long long)row * col;
            if (product > 0 && sum_nested > LLONG_MAX - product) {
                fprintf(stderr, "Integer overflow detected\n");
                return 1;
            }
            sum_nested += product;
        }
    }
    printf("%lld\n", sum_nested);
    
    printf("5. Loop with break: ");
    long long sum_break = 0;
    int m = 1;
    while (1) {
        if (m > n) {
            break;
        }
        if (m > 0 && sum_break > LLONG_MAX - (long long)m * m) {
            fprintf(stderr, "Integer overflow detected\n");
            return 1;
        }
        sum_break += (long long)m * m;
        m++;
    }
    printf("%lld\n", sum_break);
    
    printf("6. Loop with continue: ");
    long long sum_continue = 0;
    for (int p = 1; p <= n; p++) {
        if (p % 2 == 0) {
            continue;
        }
        if (p > 0 && sum_continue > LLONG_MAX - (long long)p * p) {
            fprintf(stderr, "Integer overflow detected\n");
            return 1;
        }
        sum_continue += (long long)p * p;
    }
    printf("%lld\n", sum_continue);
    
    long long expected = (long long)n * (n + 1) * (2 * n + 1) / 6;
    printf("\nExpected sum of squares (1^2 + 2^2 + ... + %d^2): %lld\n", n, expected);
    
    if (sum_for == expected && sum_while == expected && sum_do == expected) {
        printf("All loop variants produced correct results!\n");
    } else {
        printf("Discrepancy detected in results\n");
        return 1;
    }
    
    return 0;
}