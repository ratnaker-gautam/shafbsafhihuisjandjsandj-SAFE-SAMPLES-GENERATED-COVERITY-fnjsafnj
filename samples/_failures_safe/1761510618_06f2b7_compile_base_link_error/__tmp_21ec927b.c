//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define MAX_INPUT 1000000

double compute_series_sum(int n) {
    double sum = 0.0;
    for (int i = 1; i <= n; i++) {
        sum += 1.0 / (i * i);
    }
    return sum;
}

double compute_series_product(int n) {
    double product = 1.0;
    int i = 1;
    while (i <= n) {
        product *= (1.0 + 1.0 / (i * i));
        i++;
    }
    return product;
}

double compute_alternating_sum(int n) {
    double sum = 0.0;
    int sign = 1;
    int k = 1;
    do {
        sum += sign * (1.0 / k);
        sign = -sign;
        k++;
    } while (k <= n);
    return sum;
}

int main() {
    int n;
    char input_buffer[100];
    
    printf("Enter a positive integer (1-%d): ", MAX_INPUT);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (sscanf(input_buffer, "%d", &n) != 1) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    if (n < 1 || n > MAX_INPUT) {
        fprintf(stderr, "Input out of range\n");
        return 1;
    }
    
    if (n > MAX_ITERATIONS) {
        fprintf(stderr, "Input too large for computation\n");
        return 1;
    }
    
    double sum_result = compute_series_sum(n);
    double product_result = compute_series_product(n);
    double alternating_result = compute_alternating_sum(n);
    
    printf("Sum of 1/k^2 from k=1 to %d: %.10f\n", n, sum_result);
    printf("Product of (1 + 1/k^2) from k=1 to %d: %.10f\n", n, product_result);
    printf("Alternating sum of 1/k from k=1 to %d: %.10f\n", n, alternating_result);
    
    double pi_approximation = sqrt(6.0 * sum_result);
    printf("Approximation of pi using sum: %.10f\n", pi_approximation);
    
    return 0;
}