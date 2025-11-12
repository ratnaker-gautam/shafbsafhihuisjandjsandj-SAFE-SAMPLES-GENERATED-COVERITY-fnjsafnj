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
    
    if (n < 1 || n > MAX_INPUT) {
        fprintf(stderr, "Error: Input out of range\n");
        return 1;
    }
    
    printf("Computing sum of squares using different loop variants:\n\n");
    
    long long sum1 = 0;
    int i = 1;
    while (i <= n) {
        if (i > 0 && sum1 > LLONG_MAX / (long long)i) {
            fprintf(stderr, "Error: Integer overflow detected\n");
            return 1;
        }
        long long square = (long long)i * i;
        if (sum1 > LLONG_MAX - square) {
            fprintf(stderr, "Error: Integer overflow detected\n");
            return 1;
        }
        sum1 += square;
        i++;
    }
    printf("While loop result: %lld\n", sum1);
    
    long long sum2 = 0;
    for (int j = 1; j <= n; j++) {
        if (j > 0 && sum2 > LLONG_MAX / (long long)j) {
            fprintf(stderr, "Error: Integer overflow detected\n");
            return 1;
        }
        long long square = (long long)j * j;
        if (sum2 > LLONG_MAX - square) {
            fprintf(stderr, "Error: Integer overflow detected\n");
            return 1;
        }
        sum2 += square;
    }
    printf("For loop result: %lld\n", sum2);
    
    long long sum3 = 0;
    int k = 1;
    do {
        if (k > 0 && sum3 > LLONG_MAX / (long long)k) {
            fprintf(stderr, "Error: Integer overflow detected\n");
            return 1;
        }
        long long square = (long long)k * k;
        if (sum3 > LLONG_MAX - square) {
            fprintf(stderr, "Error: Integer overflow detected\n");
            return 1;
        }
        sum3 += square;
        k++;
    } while (k <= n);
    printf("Do-while loop result: %lld\n", sum3);
    
    long long sum4 = 0;
    int m = 1;
    for (;;) {
        if (m > n) break;
        
        if (m > 0 && sum4 > LLONG_MAX / (long long)m) {
            fprintf(stderr, "Error: Integer overflow detected\n");
            return 1;
        }
        long long square = (long long)m * m;
        if (sum4 > LLONG_MAX - square) {
            fprintf(stderr, "Error: Integer overflow detected\n");
            return 1;
        }
        sum4 += square;
        m++;
    }
    printf("Infinite for loop with break result: %lld\n", sum4);
    
    if (sum1 == sum2 && sum2 == sum3 && sum3 == sum4) {
        printf("\nAll loop variants produced the same result: %lld\n", sum1);
    } else {
        fprintf(stderr, "Error: Loop variants produced different results\n");
        return 1;
    }
    
    return 0;
}