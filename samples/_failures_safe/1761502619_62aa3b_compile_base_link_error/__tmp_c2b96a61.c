//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define TOLERANCE 1e-10

double compute_series_sum(int n) {
    if (n <= 0) return 0.0;
    
    double sum = 0.0;
    for (int i = 1; i <= n; i++) {
        double term = 1.0 / (i * i);
        if (term < TOLERANCE) break;
        sum += term;
    }
    return sum;
}

double compute_product(int n) {
    if (n <= 0) return 1.0;
    
    double product = 1.0;
    int i = 1;
    while (i <= n) {
        double factor = 1.0 + (1.0 / (i * i));
        if (factor > 1e6) break;
        product *= factor;
        i++;
    }
    return product;
}

double iterative_sqrt(double x) {
    if (x < 0.0) return NAN;
    if (x == 0.0) return 0.0;
    
    double guess = x / 2.0;
    int iterations = 0;
    
    do {
        double new_guess = 0.5 * (guess + x / guess);
        if (fabs(new_guess - guess) < TOLERANCE) break;
        guess = new_guess;
        iterations++;
    } while (iterations < MAX_ITERATIONS);
    
    return guess;
}

int main() {
    int n;
    printf("Enter a positive integer (1-1000): ");
    
    if (scanf("%d", &n) != 1) {
        printf("Invalid input. Must be an integer.\n");
        return 1;
    }
    
    if (n < 1 || n > 1000) {
        printf("Input out of range. Must be between 1 and 1000.\n");
        return 1;
    }
    
    double series_sum = compute_series_sum(n);
    double product = compute_product(n);
    double sqrt_result = iterative_sqrt((double)n);
    
    printf("Series sum for n=%d: %.10f\n", n, series_sum);
    printf("Product for n=%d: %.10f\n", n, product);
    printf("Square root of %d: %.10f\n", n, sqrt_result);
    
    double pi_estimate = sqrt(series_sum * 6.0);
    printf("Pi estimate from series: %.10f\n", pi_estimate);
    
    return 0;
}