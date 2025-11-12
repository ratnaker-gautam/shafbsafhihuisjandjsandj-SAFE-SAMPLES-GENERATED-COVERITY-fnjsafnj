//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define PRECISION 1e-10

double compute_series_sum(int n) {
    if (n <= 0) return 0.0;
    
    double sum = 0.0;
    for (int i = 1; i <= n; i++) {
        if (i == 0) continue;
        double term = 1.0 / ((double)i * (double)i);
        if (term < PRECISION) break;
        if (sum > 1e308 - term) break;
        sum += term;
    }
    return sum;
}

double compute_product(int n) {
    if (n <= 0) return 1.0;
    
    double product = 1.0;
    int i = 1;
    while (i <= n) {
        if (i == 0) {
            i++;
            continue;
        }
        double factor = 1.0 + (1.0 / ((double)i * (double)i));
        if (factor > 1e6) break;
        if (product > 1e308 / factor) break;
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
    do {
        if (k == 0) {
            k++;
            continue;
        }
        double denominator = (double)k * (double)k * (double)k;
        if (denominator == 0.0) {
            k++;
            continue;
        }
        double term = (double)sign / denominator;
        if (fabs(term) < PRECISION) break;
        if (sign > 0 && sum > 1e308 - term) break;
        if (sign < 0 && sum < -1e308 - term) break;
        sum += term;
        sign = -sign;
        k++;
    } while (k <= n && k <= MAX_ITERATIONS);
    return sum;
}

int main(void) {
    int n;
    printf("Enter a positive integer (1-100): ");
    
    if (scanf("%d", &n) != 1) {
        printf("Error: Invalid input\n");
        return 1;
    }
    
    if (n < 1 || n > 100) {
        printf("Error: Input must be between 1 and 100\n");
        return 1;
    }
    
    double series_sum = compute_series_sum(n);
    double product = compute_product(n);
    double alternating_sum = compute_alternating_sum(n);
    
    printf("Series sum (1/k^2): %.10f\n", series_sum);
    printf("Product (1 + 1/k^2): %.10f\n", product);
    printf("Alternating sum ((-1)^(k+1)/k^3): %.10f\n", alternating_sum);
    
    double reference = 3.14159265358979323846 * 3.14159265358979323846 / 6.0;
    printf("Reference (π²/6): %.10f\n", reference);
    printf("Difference: %.10f\n", fabs(series_sum - reference));
    
    return 0;
}