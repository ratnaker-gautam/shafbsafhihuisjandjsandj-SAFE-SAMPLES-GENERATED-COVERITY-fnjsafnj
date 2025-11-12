//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define MAX_INPUT 1000000

int main() {
    int n;
    double sum = 0.0;
    double product = 1.0;
    
    printf("Enter a positive integer (1-%d): ", MAX_INPUT);
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (n < 1 || n > MAX_INPUT) {
        fprintf(stderr, "Error: Number out of range\n");
        return 1;
    }
    
    printf("Computing sum and product using different loop variants...\n");
    
    int i = 1;
    while (i <= n) {
        sum += sqrt(i);
        i++;
    }
    
    for (int j = 1; j <= n; j++) {
        product *= 1.0 + (1.0 / j);
    }
    
    double harmonic_sum = 0.0;
    int k = 1;
    do {
        harmonic_sum += 1.0 / k;
        k++;
    } while (k <= n);
    
    double geometric_sum = 0.0;
    double term = 1.0;
    for (int m = 0; m < n; m++) {
        geometric_sum += term;
        term *= 0.5;
    }
    
    double factorial_approx = 1.0;
    int p = 1;
    while (p <= n) {
        factorial_approx *= p;
        if (factorial_approx > 1e100) {
            factorial_approx = 1e100;
            break;
        }
        p++;
    }
    
    printf("Results for n = %d:\n", n);
    printf("Sum of square roots: %.6f\n", sum);
    printf("Product (1+1/i): %.6f\n", product);
    printf("Harmonic sum: %.6f\n", harmonic_sum);
    printf("Geometric sum: %.6f\n", geometric_sum);
    printf("Factorial approximation: %.6e\n", factorial_approx);
    
    double final_result = (sum + product + harmonic_sum + geometric_sum) / 4.0;
    printf("Final combined result: %.6f\n", final_result);
    
    return 0;
}