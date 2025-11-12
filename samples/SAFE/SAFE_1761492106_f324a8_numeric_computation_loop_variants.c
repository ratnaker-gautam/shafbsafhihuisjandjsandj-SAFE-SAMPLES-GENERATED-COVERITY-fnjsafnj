//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000
#define MAX_INPUT 1000000

int main() {
    int n;
    printf("Enter a positive integer (1-%d): ", MAX_INPUT);
    
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (n <= 0 || n > MAX_INPUT) {
        fprintf(stderr, "Error: Input out of range\n");
        return 1;
    }
    
    printf("Computing sum of first %d triangular numbers using different loops:\n", n);
    
    long long sum_for = 0;
    for (int i = 1; i <= n; i++) {
        if (i > 0 && sum_for > LLONG_MAX - (long long)i * (i + 1) / 2) {
            fprintf(stderr, "Error: Integer overflow detected\n");
            return 1;
        }
        sum_for += (long long)i * (i + 1) / 2;
    }
    printf("For loop result: %lld\n", sum_for);
    
    long long sum_while = 0;
    int j = 1;
    while (j <= n) {
        if (j > 0 && sum_while > LLONG_MAX - (long long)j * (j + 1) / 2) {
            fprintf(stderr, "Error: Integer overflow detected\n");
            return 1;
        }
        sum_while += (long long)j * (j + 1) / 2;
        j++;
    }
    printf("While loop result: %lld\n", sum_while);
    
    long long sum_do_while = 0;
    int k = 1;
    if (n >= 1) {
        do {
            if (k > 0 && sum_do_while > LLONG_MAX - (long long)k * (k + 1) / 2) {
                fprintf(stderr, "Error: Integer overflow detected\n");
                return 1;
            }
            sum_do_while += (long long)k * (k + 1) / 2;
            k++;
        } while (k <= n);
    }
    printf("Do-while loop result: %lld\n", sum_do_while);
    
    long long sum_goto = 0;
    int m = 1;
loop_start:
    if (m <= n) {
        if (m > 0 && sum_goto > LLONG_MAX - (long long)m * (m + 1) / 2) {
            fprintf(stderr, "Error: Integer overflow detected\n");
            return 1;
        }
        sum_goto += (long long)m * (m + 1) / 2;
        m++;
        goto loop_start;
    }
    printf("Goto loop result: %lld\n", sum_goto);
    
    if (sum_for == sum_while && sum_while == sum_do_while && sum_do_while == sum_goto) {
        printf("All loop variants produced the same result: %lld\n", sum_for);
    } else {
        fprintf(stderr, "Error: Loop variants produced different results\n");
        return 1;
    }
    
    return 0;
}