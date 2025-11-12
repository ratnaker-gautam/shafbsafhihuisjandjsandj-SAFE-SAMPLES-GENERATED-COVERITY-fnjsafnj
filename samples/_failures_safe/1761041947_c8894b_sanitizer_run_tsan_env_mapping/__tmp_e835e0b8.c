//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
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

double compute_alternating_sum(int n) {
    if (n <= 0) return 0.0;
    
    double sum = 0.0;
    int sign = 1;
    int k = 1;
    double term;
    do {
        term = (double)sign / (k * k * k);
        sum += term;
        sign = -sign;
        k++;
    } while (k <= n && fabs(term) > TOLERANCE);
    return sum;
}

int main(void) {
    int n;
    printf("Enter a positive integer (1-100): ");
    
    if (scanf("%d", &n) != 1) {
        printf("Invalid input. Must be an integer.\n");
        return 1;
    }
    
    if (n < 1 || n > 100) {
        printf("Input out of range. Must be between 1 and 100.\n");
        return 1;
    }
    
    double series_sum = compute_series_sum(n);
    double product = compute_product(n);
    double alternating_sum = compute_alternating_sum(n);
    
    printf("Series sum (1/k^2): %.10f\n", series_sum);
    printf("Product (1+1/k^2): %.10f\n", product);
    printf("Alternating sum ((-1)^(k+1)/k^3): %.10f\n", alternating_sum);
    
    double final_result = series_sum + product + alternating_sum;
    printf("Combined result: %.10f\n", final_result);
    
    return 0;
}