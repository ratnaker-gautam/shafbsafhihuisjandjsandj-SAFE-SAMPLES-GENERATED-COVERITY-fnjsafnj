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
    
    printf("\nPartial sum after %d terms: %.10f\n", n, sum);
    
    double actual_limit = log(2.0);
    printf("Theoretical limit (ln 2): %.10f\n", actual_limit);
    printf("Absolute error: %.10f\n", fabs(sum - actual_limit));
    
    printf("\nComputing factorial sequence:\n");
    unsigned long long factorial = 1;
    
    for (int j = 1; j <= n; j++) {
        if (j > 20) {
            printf("Factorial too large to compute accurately\n");
            break;
        }
        
        if (factorial > ULLONG_MAX / j) {
            printf("Factorial overflow at term %d\n", j);
            break;
        }
        
        factorial *= j;
        printf("%d! = %llu\n", j, factorial);
    }
    
    printf("\nComputing square roots with different loops:\n");
    double squares[20];
    int k = 0;
    
    do {
        if (k >= n) break;
        squares[k] = sqrt(k + 1);
        printf("sqrt(%d) = %.6f\n", k + 1, squares[k]);
        k++;
    } while (k < n);
    
    printf("\nComputing cumulative sum of squares:\n");
    double cumulative = 0.0;
    
    for (int m = 0; m < n; m++) {
        cumulative += squares[m] * squares[m];
        printf("After %d terms: %.6f\n", m + 1, cumulative);
    }
    
    return 0;
}