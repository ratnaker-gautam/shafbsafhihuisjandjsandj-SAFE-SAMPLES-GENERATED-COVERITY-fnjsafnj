//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
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
    
    long long sum1 = 0;
    int i = 1;
    while (i <= n) {
        if (i > 0 && sum1 > LLONG_MAX / (long long)i) {
            fprintf(stderr, "Integer overflow detected\n");
            return 1;
        }
        long long square = (long long)i * i;
        if (sum1 > LLONG_MAX - square) {
            fprintf(stderr, "Integer overflow detected\n");
            return 1;
        }
        sum1 += square;
        i++;
    }
    printf("While loop sum: %lld\n", sum1);
    
    long long sum2 = 0;
    int j = 1;
    do {
        if (j > 0 && sum2 > LLONG_MAX / (long long)j) {
            fprintf(stderr, "Integer overflow detected\n");
            return 1;
        }
        long long square = (long long)j * j;
        if (sum2 > LLONG_MAX - square) {
            fprintf(stderr, "Integer overflow detected\n");
            return 1;
        }
        sum2 += square;
        j++;
    } while (j <= n);
    printf("Do-while loop sum: %lld\n", sum2);
    
    long long sum3 = 0;
    for (int k = 1; k <= n; k++) {
        if (k > 0 && sum3 > LLONG_MAX / (long long)k) {
            fprintf(stderr, "Integer overflow detected\n");
            return 1;
        }
        long long square = (long long)k * k;
        if (sum3 > LLONG_MAX - square) {
            fprintf(stderr, "Integer overflow detected\n");
            return 1;
        }
        sum3 += square;
    }
    printf("For loop sum: %lld\n", sum3);
    
    long long sum4 = 0;
    int m = 1;
    for (;;) {
        if (m > n) break;
        if (m > 0 && sum4 > LLONG_MAX / (long long)m) {
            fprintf(stderr, "Integer overflow detected\n");
            return 1;
        }
        long long square = (long long)m * m;
        if (sum4 > LLONG_MAX - square) {
            fprintf(stderr, "Integer overflow detected\n");
            return 1;
        }
        sum4 += square;
        m++;
    }
    printf("Infinite for loop with break: %lld\n", sum4);
    
    long long expected = (long long)n * (n + 1) * (2 * n + 1) / 6;
    printf("\nExpected result (formula): %lld\n", expected);
    
    if (sum1 == expected && sum2 == expected && sum3 == expected && sum4 == expected) {
        printf("All loop variants produced correct results!\n");
    } else {
        printf("Error: Loop variants produced inconsistent results\n");
        return 1;
    }
    
    return 0;
}