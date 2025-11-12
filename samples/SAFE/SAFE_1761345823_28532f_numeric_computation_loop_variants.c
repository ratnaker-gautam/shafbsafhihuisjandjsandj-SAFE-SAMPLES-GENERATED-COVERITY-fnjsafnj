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
        if (isinf(term) || isnan(term)) {
            fprintf(stderr, "Numerical overflow detected\n");
            exit(EXIT_FAILURE);
        }
        sum += term;
        if (isinf(sum) || isnan(sum)) {
            fprintf(stderr, "Numerical overflow in summation\n");
            exit(EXIT_FAILURE);
        }
    }
    return sum;
}

double compute_product(int n) {
    double product = 1.0;
    int i = 1;
    while (i <= n) {
        double factor = 1.0 + (1.0 / (i * i));
        if (isinf(factor) || isnan(factor)) {
            fprintf(stderr, "Numerical overflow in factor\n");
            exit(EXIT_FAILURE);
        }
        product *= factor;
        if (isinf(product) || isnan(product)) {
            fprintf(stderr, "Numerical overflow in product\n");
            exit(EXIT_FAILURE);
        }
        i++;
    }
    return product;
}

double compute_alternating_sum(int n) {
    double sum = 0.0;
    int sign = 1;
    int k = 1;
    do {
        if (k > n) break;
        double term = sign * (1.0 / k);
        if (isinf(term) || isnan(term)) {
            fprintf(stderr, "Numerical overflow in alternating term\n");
            exit(EXIT_FAILURE);
        }
        sum += term;
        if (isinf(sum) || isnan(sum)) {
            fprintf(stderr, "Numerical overflow in alternating sum\n");
            exit(EXIT_FAILURE);
        }
        sign = -sign;
        k++;
    } while (k <= n);
    return sum;
}

int main() {
    int n;
    printf("Enter a positive integer (1-1000): ");
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Invalid input: must be an integer\n");
        return EXIT_FAILURE;
    }
    
    if (n < 1 || n > MAX_ITERATIONS) {
        fprintf(stderr, "Input out of range: must be between 1 and %d\n", MAX_ITERATIONS);
        return EXIT_FAILURE;
    }
    
    double series_sum = compute_series_sum(n);
    double product = compute_product(n);
    double alternating_sum = compute_alternating_sum(n);
    
    printf("Sum of 1/k^2 from k=1 to %d: %.10f\n", n, series_sum);
    printf("Product of (1 + 1/k^2) from k=1 to %d: %.10f\n", n, product);
    printf("Alternating sum (-1)^(k+1)/k from k=1 to %d: %.10f\n", n, alternating_sum);
    
    return EXIT_SUCCESS;
}