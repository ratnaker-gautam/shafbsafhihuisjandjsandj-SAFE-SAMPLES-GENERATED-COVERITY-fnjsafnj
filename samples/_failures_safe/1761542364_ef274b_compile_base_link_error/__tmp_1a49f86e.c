//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10

double compute_series_sum(int n) {
    if (n <= 0) return 0.0;
    
    double sum = 0.0;
    for (int i = 1; i <= n; i++) {
        double term = 1.0 / (i * i);
        if (term < EPSILON) break;
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

double compute_alternating_sum(int n) {
    if (n <= 0) return 0.0;
    
    double sum = 0.0;
    int sign = 1;
    int i = 1;
    do {
        double term = sign * (1.0 / i);
        if (fabs(term) < EPSILON) break;
        sum += term;
        sign = -sign;
        i++;
    } while (i <= n);
    return sum;
}

int main(void) {
    int n;
    printf("Enter a positive integer (1-1000): ");
    
    if (scanf("%d", &n) != 1) {
        printf("Error: Invalid input\n");
        return 1;
    }
    
    if (n < 1 || n > 1000) {
        printf("Error: Input must be between 1 and 1000\n");
        return 1;
    }
    
    double series_sum = compute_series_sum(n);
    double product = compute_product(n);
    double alternating_sum = compute_alternating_sum(n);
    
    printf("Series sum (1/i^2): %.10f\n", series_sum);
    printf("Product (1 + 1/i^2): %.10f\n", product);
    printf("Alternating sum ((-1)^(i+1)/i): %.10f\n", alternating_sum);
    
    double pi_approximation = sqrt(6.0 * series_sum);
    printf("Pi approximation from series: %.10f\n", pi_approximation);
    
    return 0;
}