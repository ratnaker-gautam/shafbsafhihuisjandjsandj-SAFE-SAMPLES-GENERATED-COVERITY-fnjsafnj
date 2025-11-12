//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define PRECISION 1e-10

double compute_series_sum(int n) {
    if (n <= 0) return 0.0;
    
    double sum = 0.0;
    int i = 1;
    while (i <= n) {
        double term = 1.0 / (i * i);
        if (term < PRECISION) break;
        sum += term;
        i++;
    }
    return sum;
}

double compute_product(int n) {
    if (n <= 0) return 1.0;
    
    double product = 1.0;
    for (int i = 1; i <= n; i++) {
        double factor = 1.0 + (1.0 / (i * i));
        if (factor > 1e6 || factor < 1e-6) break;
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
        double term = sign * (1.0 / i);
        if (fabs(term) < PRECISION) break;
        sum += term;
        sign = -sign;
        i++;
    } while (i <= n && i <= MAX_ITERATIONS);
    return sum;
}

int main(void) {
    int n;
    char input[100];
    
    printf("Enter a positive integer (1-1000): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (sscanf(input, "%d", &n) != 1) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    if (n < 1 || n > 1000) {
        fprintf(stderr, "Input out of range\n");
        return 1;
    }
    
    double series_sum = compute_series_sum(n);
    double product = compute_product(n);
    double alternating_sum = compute_alternating_sum(n);
    
    printf("Series sum (1/k^2): %.10f\n", series_sum);
    printf("Product (1 + 1/k^2): %.10f\n", product);
    printf("Alternating sum ((-1)^(k+1)/k): %.10f\n", alternating_sum);
    
    return 0;
}