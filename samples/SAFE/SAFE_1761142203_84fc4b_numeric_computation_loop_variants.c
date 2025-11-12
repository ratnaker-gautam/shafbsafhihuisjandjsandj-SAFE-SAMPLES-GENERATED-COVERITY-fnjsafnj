//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT 100

int main(void) {
    char input[MAX_INPUT + 1];
    int n;
    
    printf("Enter number of terms to compute (1-20): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    if (sscanf(input, "%d", &n) != 1) {
        fprintf(stderr, "Invalid number format\n");
        return 1;
    }
    
    if (n < 1 || n > 20) {
        fprintf(stderr, "Number must be between 1 and 20\n");
        return 1;
    }
    
    printf("Computing %d terms using different loop variants:\n\n", n);
    
    printf("For loop (sum of squares): ");
    long sum_squares = 0;
    for (int i = 1; i <= n; i++) {
        if (i > INT_MAX / i) {
            fprintf(stderr, "Integer overflow detected\n");
            return 1;
        }
        sum_squares += (long)i * i;
    }
    printf("%ld\n", sum_squares);
    
    printf("While loop (factorial sequence): ");
    int j = 1;
    long factorial = 1;
    while (j <= n) {
        if (factorial > LONG_MAX / j) {
            fprintf(stderr, "Factorial overflow at term %d\n", j);
            return 1;
        }
        factorial *= j;
        printf("%ld ", factorial);
        j++;
    }
    printf("\n");
    
    printf("Do-while loop (Fibonacci sequence): ");
    if (n >= 1) printf("0 ");
    if (n >= 2) printf("1 ");
    
    long fib_prev = 0;
    long fib_curr = 1;
    int k = 3;
    do {
        if (k > n) break;
        
        if (fib_curr > LONG_MAX - fib_prev) {
            fprintf(stderr, "Fibonacci overflow at term %d\n", k);
            return 1;
        }
        long fib_next = fib_prev + fib_curr;
        printf("%ld ", fib_next);
        fib_prev = fib_curr;
        fib_curr = fib_next;
        k++;
    } while (k <= n);
    printf("\n");
    
    printf("Nested loops (multiplication table):\n");
    for (int row = 1; row <= n; row++) {
        for (int col = 1; col <= n; col++) {
            if (row > INT_MAX / col) {
                fprintf(stderr, "Multiplication overflow\n");
                return 1;
            }
            printf("%3d ", row * col);
        }
        printf("\n");
    }
    
    printf("Alternating sum with break: ");
    double alt_sum = 0.0;
    for (int m = 1; m <= n * 2; m++) {
        if (m > n * 2) break;
        
        double term = 1.0 / m;
        if (m % 2 == 0) {
            alt_sum -= term;
        } else {
            alt_sum += term;
        }
        
        if (m >= n) {
            printf("%.6f ", alt_sum);
            if (m == n * 2) break;
        }
    }
    printf("\n");
    
    return 0;
}