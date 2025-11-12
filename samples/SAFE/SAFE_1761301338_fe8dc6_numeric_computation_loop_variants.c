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
    
    char *endptr;
    n = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\n' && *endptr != '\0') {
        fprintf(stderr, "Invalid input: not a valid integer\n");
        return 1;
    }
    
    if (n < 1 || n > 100) {
        fprintf(stderr, "Input out of range (1-100)\n");
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
    printf("sum = %lld\n", sum_for);
    
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
    printf("sum = %lld\n", sum_while);
    
    printf("3. Do-while loop: ");
    long long sum_do_while = 0;
    int k = 1;
    do {
        if (k > 0 && sum_do_while > LLONG_MAX - (long long)k * k) {
            fprintf(stderr, "Integer overflow detected\n");
            return 1;
        }
        sum_do_while += (long long)k * k;
        k++;
    } while (k <= n);
    printf("sum = %lld\n", sum_do_while);
    
    printf("4. Nested loops (sum of products): ");
    long long sum_nested = 0;
    for (int a = 1; a <= n; a++) {
        for (int b = 1; b <= a; b++) {
            if (a > 0 && b > 0 && sum_nested > LLONG_MAX - (long long)a * b) {
                fprintf(stderr, "Integer overflow detected\n");
                return 1;
            }
            sum_nested += (long long)a * b;
        }
    }
    printf("sum = %lld\n", sum_nested);
    
    long long expected = (long long)n * (n + 1) * (2 * n + 1) / 6;
    printf("\nExpected sum of squares (formula): %lld\n", expected);
    
    if (sum_for == expected && sum_while == expected && sum_do_while == expected) {
        printf("All loop variants produced correct results!\n");
    } else {
        printf("Warning: Loop variants produced inconsistent results\n");
    }
    
    return 0;
}