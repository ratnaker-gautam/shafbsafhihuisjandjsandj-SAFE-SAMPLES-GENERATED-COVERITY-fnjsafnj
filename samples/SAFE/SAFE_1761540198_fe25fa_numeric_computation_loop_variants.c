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
        fprintf(stderr, "Error: Number out of range\n");
        return 1;
    }
    
    printf("Computing sum of first %d triangular numbers...\n", n);
    
    long long sum = 0;
    int i = 1;
    
    while (i <= n) {
        if (i > INT_MAX / 2) {
            fprintf(stderr, "Error: Integer overflow detected\n");
            return 1;
        }
        long long triangular = (long long)i * (i + 1) / 2;
        
        if (sum > LLONG_MAX - triangular) {
            fprintf(stderr, "Error: Sum overflow detected\n");
            return 1;
        }
        sum += triangular;
        i++;
    }
    
    printf("Result using while loop: %lld\n", sum);
    
    sum = 0;
    int j;
    for (j = 1; j <= n; j++) {
        if (j > INT_MAX / 2) {
            fprintf(stderr, "Error: Integer overflow detected\n");
            return 1;
        }
        long long triangular = (long long)j * (j + 1) / 2;
        
        if (sum > LLONG_MAX - triangular) {
            fprintf(stderr, "Error: Sum overflow detected\n");
            return 1;
        }
        sum += triangular;
    }
    
    printf("Result using for loop: %lld\n", sum);
    
    sum = 0;
    int k = 1;
    do {
        if (k > INT_MAX / 2) {
            fprintf(stderr, "Error: Integer overflow detected\n");
            return 1;
        }
        long long triangular = (long long)k * (k + 1) / 2;
        
        if (sum > LLONG_MAX - triangular) {
            fprintf(stderr, "Error: Sum overflow detected\n");
            return 1;
        }
        sum += triangular;
        k++;
    } while (k <= n);
    
    printf("Result using do-while loop: %lld\n", sum);
    
    sum = 0;
    int m = 1;
    start:
    if (m <= n) {
        if (m > INT_MAX / 2) {
            fprintf(stderr, "Error: Integer overflow detected\n");
            return 1;
        }
        long long triangular = (long long)m * (m + 1) / 2;
        
        if (sum > LLONG_MAX - triangular) {
            fprintf(stderr, "Error: Sum overflow detected\n");
            return 1;
        }
        sum += triangular;
        m++;
        goto start;
    }
    
    printf("Result using goto loop: %lld\n", sum);
    
    return 0;
}