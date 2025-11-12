//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define TOLERANCE 1e-10

double compute_series_sum(int n) {
    double sum = 0.0;
    for (int i = 1; i <= n; i++) {
        if (i == 0) continue;
        double term = 1.0 / ((double)i * (double)i);
        if (term < TOLERANCE) break;
        sum += term;
    }
    return sum;
}

double compute_product(int n) {
    double product = 1.0;
    int i = 1;
    while (i <= n) {
        if (i == 0) {
            i++;
            continue;
        }
        double factor = 1.0 + (1.0 / ((double)i * (double)i));
        if (factor > 1e6) break;
        product *= factor;
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
        if (k == 0) {
            k++;
            continue;
        }
        double term = sign * (1.0 / (double)k);
        sum += term;
        sign = -sign;
        k++;
    } while (k <= MAX_ITERATIONS);
    return sum;
}

int main() {
    char input[32];
    int n;
    
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
    printf("Product (1+1/k^2): %.10f\n", product);
    printf("Alternating sum ((-1)^(k+1)/k): %.10f\n", alternating_sum);
    printf("Theoretical limit (π^2/6): %.10f\n", (3.14159265358979323846 * 3.14159265358979323846) / 6.0);
    
    return 0;
}