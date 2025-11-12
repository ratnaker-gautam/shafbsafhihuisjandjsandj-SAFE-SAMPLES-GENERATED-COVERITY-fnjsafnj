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
        fprintf(stderr, "Invalid input: must be between 1 and 100\n");
        return 1;
    }
    
    printf("Computing sum of squares using different loop variants:\n\n");
    
    long long sum1 = 0;
    int i = 1;
    while (i <= n) {
        if (i > 0 && sum1 > LLONG_MAX - (long long)i * i) {
            fprintf(stderr, "Integer overflow detected\n");
            return 1;
        }
        sum1 += (long long)i * i;
        i++;
    }
    printf("While loop result: %lld\n", sum1);
    
    long long sum2 = 0;
    for (int j = 1; j <= n; j++) {
        if (j > 0 && sum2 > LLONG_MAX - (long long)j * j) {
            fprintf(stderr, "Integer overflow detected\n");
            return 1;
        }
        sum2 += (long long)j * j;
    }
    printf("For loop result: %lld\n", sum2);
    
    long long sum3 = 0;
    int k = 1;
    do {
        if (k > 0 && sum3 > LLONG_MAX - (long long)k * k) {
            fprintf(stderr, "Integer overflow detected\n");
            return 1;
        }
        sum3 += (long long)k * k;
        k++;
    } while (k <= n);
    printf("Do-while loop result: %lld\n", sum3);
    
    long long sum4 = 0;
    int m = 1;
    loop_start:
    if (m <= n) {
        if (m > 0 && sum4 > LLONG_MAX - (long long)m * m) {
            fprintf(stderr, "Integer overflow detected\n");
            return 1;
        }
        sum4 += (long long)m * m;
        m++;
        goto loop_start;
    }
    printf("Goto loop result: %lld\n", sum4);
    
    long long expected = (long long)n * (n + 1) * (2 * n + 1) / 6;
    printf("Expected result (formula): %lld\n", expected);
    
    if (sum1 == sum2 && sum2 == sum3 && sum3 == sum4 && sum4 == expected) {
        printf("All loop variants produced correct results!\n");
    } else {
        printf("Warning: Results don't match!\n");
    }
    
    return 0;
}