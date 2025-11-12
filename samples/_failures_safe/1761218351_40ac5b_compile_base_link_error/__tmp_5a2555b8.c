//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10

double compute_series_sum(int n) {
    double sum = 0.0;
    for (int i = 1; i <= n; i++) {
        double term = 1.0 / (i * i);
        if (term < EPSILON) break;
        sum += term;
    }
    return sum;
}

double compute_product(int n) {
    double product = 1.0;
    int i = 1;
    while (i <= n) {
        double factor = 1.0 + (1.0 / (i * i));
        product *= factor;
        i++;
    }
    return product;
}

double compute_alternating_sum(int n) {
    double sum = 0.0;
    int sign = 1;
    for (int i = 1; i <= n; i++) {
        double term = sign * (1.0 / i);
        sum += term;
        sign = -sign;
    }
    return sum;
}

int main() {
    int n;
    printf("Enter a positive integer (1-%d): ", MAX_ITERATIONS);
    
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (n < 1 || n > MAX_ITERATIONS) {
        fprintf(stderr, "Error: Input out of range\n");
        return 1;
    }
    
    double series_sum = compute_series_sum(n);
    double product = compute_product(n);
    double alternating_sum = compute_alternating_sum(n);
    
    printf("Sum of 1/i^2 series: %.10f\n", series_sum);
    printf("Product of (1 + 1/i^2) series: %.10f\n", product);
    printf("Alternating harmonic series: %.10f\n", alternating_sum);
    
    double pi_estimate = sqrt(6.0 * series_sum);
    printf("Pi estimate from series: %.10f\n", pi_estimate);
    
    return 0;
}