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
    
    char *endptr;
    long val = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\n') {
        fprintf(stderr, "Invalid input: not a valid integer\n");
        return 1;
    }
    
    if (val <= 0 || val > MAX_INPUT) {
        fprintf(stderr, "Invalid input: must be between 1 and %d\n", MAX_INPUT);
        return 1;
    }
    
    n = (int)val;
    
    printf("Computing sum of squares using different loop variants:\n\n");
    
    int sum1 = 0;
    int i = 1;
    while (i <= n) {
        if (i > 0 && i <= n) {
            int square = i * i;
            if (sum1 > INT_MAX - square) {
                fprintf(stderr, "Integer overflow detected\n");
                return 1;
            }
            sum1 += square;
        }
        i++;
    }
    printf("While loop sum: %d\n", sum1);
    
    int sum2 = 0;
    for (int j = 1; j <= n; j++) {
        if (j > 0 && j <= n) {
            int square = j * j;
            if (sum2 > INT_MAX - square) {
                fprintf(stderr, "Integer overflow detected\n");
                return 1;
            }
            sum2 += square;
        }
    }
    printf("For loop sum: %d\n", sum2);
    
    int sum3 = 0;
    int k = 1;
    do {
        if (k > 0 && k <= n) {
            int square = k * k;
            if (sum3 > INT_MAX - square) {
                fprintf(stderr, "Integer overflow detected\n");
                return 1;
            }
            sum3 += square;
        }
        k++;
    } while (k <= n);
    printf("Do-while loop sum: %d\n", sum3);
    
    int sum4 = 0;
    int m = 1;
    start:
    if (m <= n) {
        if (m > 0 && m <= n) {
            int square = m * m;
            if (sum4 > INT_MAX - square) {
                fprintf(stderr, "Integer overflow detected\n");
                return 1;
            }
            sum4 += square;
        }
        m++;
        goto start;
    }
    printf("Goto loop sum: %d\n", sum4);
    
    if (sum1 == sum2 && sum2 == sum3 && sum3 == sum4) {
        printf("\nAll loop variants produced the same result: %d\n", sum1);
        printf("Mathematical formula: n(n+1)(2n+1)/6 = %d\n", (n * (n + 1) * (2 * n + 1)) / 6);
    } else {
        fprintf(stderr, "Error: Loop variants produced different results\n");
        return 1;
    }
    
    return 0;
}