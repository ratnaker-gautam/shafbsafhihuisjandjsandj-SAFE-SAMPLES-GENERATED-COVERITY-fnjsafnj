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
    
    long long sum_for = 0;
    for (int i = 1; i <= n; i++) {
        if (i > 0 && sum_for > LLONG_MAX / i) {
            fprintf(stderr, "Integer overflow detected\n");
            return 1;
        }
        long long square = (long long)i * i;
        if (sum_for > LLONG_MAX - square) {
            fprintf(stderr, "Integer overflow detected\n");
            return 1;
        }
        sum_for += square;
    }
    printf("For loop sum: %lld\n", sum_for);
    
    long long sum_while = 0;
    int j = 1;
    while (j <= n) {
        if (j > 0 && sum_while > LLONG_MAX / j) {
            fprintf(stderr, "Integer overflow detected\n");
            return 1;
        }
        long long square = (long long)j * j;
        if (sum_while > LLONG_MAX - square) {
            fprintf(stderr, "Integer overflow detected\n");
            return 1;
        }
        sum_while += square;
        j++;
    }
    printf("While loop sum: %lld\n", sum_while);
    
    long long sum_do_while = 0;
    int k = 1;
    if (n >= 1) {
        do {
            if (k > 0 && sum_do_while > LLONG_MAX / k) {
                fprintf(stderr, "Integer overflow detected\n");
                return 1;
            }
            long long square = (long long)k * k;
            if (sum_do_while > LLONG_MAX - square) {
                fprintf(stderr, "Integer overflow detected\n");
                return 1;
            }
            sum_do_while += square;
            k++;
        } while (k <= n);
    }
    printf("Do-while loop sum: %lld\n", sum_do_while);
    
    long long expected = (long long)n * (n + 1) * (2 * n + 1) / 6;
    printf("Expected sum (formula): %lld\n", expected);
    
    if (sum_for == expected && sum_while == expected && sum_do_while == expected) {
        printf("All loop variants produced correct result!\n");
    } else {
        printf("Error: Loop variants produced different results\n");
        return 1;
    }
    
    return 0;
}