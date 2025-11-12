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
        fprintf(stderr, "Input out of range\n");
        return 1;
    }
    
    printf("Computing sum of squares using different loop variants:\n\n");
    
    printf("1. For loop:\n");
    long long sum_for = 0;
    for (int i = 1; i <= n; i++) {
        if (i > 0 && sum_for <= LLONG_MAX - (long long)i * i) {
            sum_for += (long long)i * i;
        } else {
            fprintf(stderr, "Integer overflow detected\n");
            return 1;
        }
    }
    printf("Sum of squares from 1 to %d: %lld\n\n", n, sum_for);
    
    printf("2. While loop:\n");
    long long sum_while = 0;
    int j = 1;
    while (j <= n) {
        if (j > 0 && sum_while <= LLONG_MAX - (long long)j * j) {
            sum_while += (long long)j * j;
        } else {
            fprintf(stderr, "Integer overflow detected\n");
            return 1;
        }
        j++;
    }
    printf("Sum of squares from 1 to %d: %lld\n\n", n, sum_while);
    
    printf("3. Do-while loop:\n");
    long long sum_do_while = 0;
    int k = 1;
    do {
        if (k > 0 && sum_do_while <= LLONG_MAX - (long long)k * k) {
            sum_do_while += (long long)k * k;
        } else {
            fprintf(stderr, "Integer overflow detected\n");
            return 1;
        }
        k++;
    } while (k <= n);
    printf("Sum of squares from 1 to %d: %lld\n\n", n, sum_do_while);
    
    printf("4. Nested loops (sum of products):\n");
    long long sum_nested = 0;
    for (int a = 1; a <= n; a++) {
        for (int b = 1; b <= n; b++) {
            if (a > 0 && b > 0 && sum_nested <= LLONG_MAX - (long long)a * b) {
                sum_nested += (long long)a * b;
            } else {
                fprintf(stderr, "Integer overflow detected\n");
                return 1;
            }
        }
    }
    printf("Sum of products for %dx%d matrix: %lld\n\n", n, n, sum_nested);
    
    printf("Verification (mathematical formula for sum of squares):\n");
    long long formula_result = (long long)n * (n + 1) * (2 * n + 1) / 6;
    printf("n(n+1)(2n+1)/6 = %lld\n", formula_result);
    
    if (sum_for == formula_result && sum_while == formula_result && sum_do_while == formula_result) {
        printf("All loop variants produced correct results!\n");
    } else {
        printf("Results mismatch detected!\n");
        return 1;
    }
    
    return 0;
}