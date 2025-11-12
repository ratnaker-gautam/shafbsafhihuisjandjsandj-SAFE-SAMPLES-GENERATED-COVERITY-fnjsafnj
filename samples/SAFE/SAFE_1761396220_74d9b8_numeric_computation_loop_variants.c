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
        long long term = (long long)i * i;
        if (i > 0 && sum_for > LLONG_MAX - term) {
            fprintf(stderr, "Integer overflow detected\n");
            return 1;
        }
        sum_for += term;
    }
    printf("%lld\n", sum_for);
    
    printf("2. While loop: ");
    long long sum_while = 0;
    int j = 1;
    while (j <= n) {
        long long term = (long long)j * j;
        if (j > 0 && sum_while > LLONG_MAX - term) {
            fprintf(stderr, "Integer overflow detected\n");
            return 1;
        }
        sum_while += term;
        j++;
    }
    printf("%lld\n", sum_while);
    
    printf("3. Do-while loop: ");
    long long sum_do_while = 0;
    int k = 1;
    do {
        long long term = (long long)k * k;
        if (k > 0 && sum_do_while > LLONG_MAX - term) {
            fprintf(stderr, "Integer overflow detected\n");
            return 1;
        }
        sum_do_while += term;
        k++;
    } while (k <= n);
    printf("%lld\n", sum_do_while);
    
    printf("4. Nested loops (sum of products): ");
    long long sum_nested = 0;
    for (int a = 1; a <= n; a++) {
        for (int b = 1; b <= n; b++) {
            long long term = (long long)a * b;
            if (a > 0 && b > 0 && sum_nested > LLONG_MAX - term) {
                fprintf(stderr, "Integer overflow detected\n");
                return 1;
            }
            sum_nested += term;
        }
    }
    printf("%lld\n", sum_nested);
    
    printf("5. Loop with step (even numbers only): ");
    long long sum_step = 0;
    for (int m = 2; m <= n * 2; m += 2) {
        long long term = (long long)m * m;
        if (m > 0 && sum_step > LLONG_MAX - term) {
            fprintf(stderr, "Integer overflow detected\n");
            return 1;
        }
        sum_step += term;
    }
    printf("%lld\n", sum_step);
    
    printf("\nVerification (mathematical formula): ");
    long long expected = (long long)n * (n + 1) * (2 * n + 1) / 6;
    printf("%lld\n", expected);
    
    if (sum_for == expected && sum_while == expected && sum_do_while == expected) {
        printf("All loop variants produced correct results!\n");
    } else {
        printf("Results mismatch detected!\n");
        return 1;
    }
    
    return 0;
}