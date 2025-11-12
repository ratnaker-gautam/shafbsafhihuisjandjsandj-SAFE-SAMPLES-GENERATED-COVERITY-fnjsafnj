//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

#define MAX_INPUT 100

int main() {
    char input[MAX_INPUT];
    int n;
    
    printf("Enter number of terms to compute (1-20): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (sscanf(input, "%d", &n) != 1) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    if (n < 1 || n > 20) {
        fprintf(stderr, "Number must be between 1 and 20\n");
        return 1;
    }
    
    printf("Computing %d terms of alternating harmonic series:\n", n);
    
    double sum = 0.0;
    int i = 1;
    
    while (i <= n) {
        double term = 1.0 / i;
        if (i % 2 == 0) {
            sum -= term;
            printf("Term %d: -1/%d = %.6f\n", i, i, -term);
        } else {
            sum += term;
            printf("Term %d: +1/%d = %.6f\n", i, i, term);
        }
        i++;
    }
    
    printf("\nSum after %d terms: %.10f\n", n, sum);
    
    printf("\nComputing factorial sequence:\n");
    for (int j = 1; j <= n; j++) {
        long long fact = 1;
        int k = 1;
        do {
            if (k > 20) break;
            if (fact > LLONG_MAX / k) {
                fprintf(stderr, "Factorial overflow at term %d\n", j);
                return 1;
            }
            fact *= k;
            k++;
        } while (k <= j);
        printf("%d! = %lld\n", j, fact);
    }
    
    printf("\nComputing square roots with different loops:\n");
    double squares[20];
    int idx = 0;
    
    for (int m = 1; m <= n; m++) {
        squares[idx++] = sqrt(m);
    }
    
    idx = 0;
    while (idx < n) {
        printf("sqrt(%d) = %.6f\n", idx + 1, squares[idx]);
        idx++;
    }
    
    printf("\nComputing powers of 2:\n");
    int count = 0;
    unsigned long power = 1;
    
    for (;;) {
        if (count >= n) break;
        printf("2^%d = %lu\n", count, power);
        if (power > ULONG_MAX / 2) {
            fprintf(stderr, "Power overflow detected\n");
            return 1;
        }
        power *= 2;
        count++;
    }
    
    return 0;
}