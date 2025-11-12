//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define TOLERANCE 1e-10

double compute_series_sum(int n) {
    double sum = 0.0;
    for (int i = 1; i <= n; i++) {
        double term = 1.0 / (i * i);
        if (term < TOLERANCE) break;
        sum += term;
    }
    return sum;
}

double compute_alternating_sum(int n) {
    double sum = 0.0;
    int i = 1;
    while (i <= n) {
        double term = (i % 2 == 0 ? -1.0 : 1.0) / i;
        if (fabs(term) < TOLERANCE) break;
        sum += term;
        i++;
    }
    return sum;
}

double compute_product(int n) {
    double product = 1.0;
    int i = 1;
    do {
        double factor = 1.0 + 1.0 / (i * i);
        if (factor < 1.0 + TOLERANCE && factor > 1.0 - TOLERANCE) break;
        product *= factor;
        i++;
    } while (i <= n);
    return product;
}

int main() {
    int n;
    printf("Enter number of terms (1-1000): ");
    
    if (scanf("%d", &n) != 1) {
        printf("Invalid input. Must be an integer.\n");
        return 1;
    }
    
    if (n < 1 || n > 1000) {
        printf("Input out of range. Must be between 1 and 1000.\n");
        return 1;
    }
    
    double sum1 = compute_series_sum(n);
    double sum2 = compute_alternating_sum(n);
    double product = compute_product(n);
    
    printf("Series sum (1/i^2): %.10f\n", sum1);
    printf("Alternating sum ((-1)^(i+1)/i): %.10f\n", sum2);
    printf("Product (1 + 1/i^2): %.10f\n", product);
    
    double pi_approx = sqrt(6.0 * sum1);
    printf("Pi approximation (sqrt(6*series_sum)): %.10f\n", pi_approx);
    
    return 0;
}