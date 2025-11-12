//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10

double compute_series_sum(int n) {
    if (n <= 0) return 0.0;
    
    double sum = 0.0;
    int i = 1;
    while (i <= n) {
        double term = 1.0 / (i * i);
        if (term < EPSILON) break;
        sum += term;
        i++;
    }
    return sum;
}

double compute_product(int n) {
    if (n <= 0) return 1.0;
    
    double product = 1.0;
    for (int i = 1; i <= n; i++) {
        double factor = 1.0 + (1.0 / i);
        product *= factor;
    }
    return product;
}

double compute_alternating_sum(int n) {
    if (n <= 0) return 0.0;
    
    double sum = 0.0;
    int sign = 1;
    int i = 1;
    do {
        double term = (double)sign / i;
        sum += term;
        sign = -sign;
        i++;
    } while (i <= n);
    return sum;
}

int main() {
    int n;
    printf("Enter a positive integer (1-1000): ");
    
    if (scanf("%d", &n) != 1) {
        printf("Error: Invalid input\n");
        return 1;
    }
    
    if (n < 1 || n > MAX_ITERATIONS) {
        printf("Error: Input must be between 1 and %d\n", MAX_ITERATIONS);
        return 1;
    }
    
    double series_sum = compute_series_sum(n);
    double product = compute_product(n);
    double alternating_sum = compute_alternating_sum(n);
    
    printf("Series sum (1/i^2) for n=%d: %.10f\n", n, series_sum);
    printf("Product (1+1/i) for n=%d: %.10f\n", n, product);
    printf("Alternating sum ((-1)^(i+1)/i) for n=%d: %.10f\n", n, alternating_sum);
    
    return 0;
}