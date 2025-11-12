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
        sum += term;
    }
    return sum;
}

double compute_series_sum_while(int n) {
    double sum = 0.0;
    int i = 1;
    while (i <= n) {
        if (i == 0) {
            i++;
            continue;
        }
        double term = 1.0 / ((double)i * (double)i);
        sum += term;
        i++;
    }
    return sum;
}

double compute_series_sum_do_while(int n) {
    double sum = 0.0;
    int i = 1;
    if (n < 1) return 0.0;
    do {
        if (i == 0) {
            i++;
            continue;
        }
        double term = 1.0 / ((double)i * (double)i);
        sum += term;
        i++;
    } while (i <= n);
    return sum;
}

int main() {
    int n;
    printf("Enter number of terms for series computation: ");
    
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (n < 1) {
        fprintf(stderr, "Error: Number of terms must be positive\n");
        return 1;
    }
    
    if (n > MAX_ITERATIONS) {
        fprintf(stderr, "Error: Number of terms exceeds maximum allowed\n");
        return 1;
    }
    
    double result_for = compute_series_sum(n);
    double result_while = compute_series_sum_while(n);
    double result_do_while = compute_series_sum_do_while(n);
    
    printf("Series sum using for loop: %.10f\n", result_for);
    printf("Series sum using while loop: %.10f\n", result_while);
    printf("Series sum using do-while loop: %.10f\n", result_do_while);
    
    double expected = 3.14159265358979323846 * 3.14159265358979323846 / 6.0;
    double partial_sum = compute_series_sum(n);
    double error = fabs(expected - partial_sum);
    
    printf("Expected limit (pi^2/6): %.10f\n", expected);
    printf("Error for %d terms: %.10f\n", n, error);
    
    int iterations = 0;
    double current_sum = 0.0;
    double prev_sum = 0.0;
    
    for (int i = 1; i <= MAX_ITERATIONS; i++) {
        if (i == 0) continue;
        current_sum += 1.0 / ((double)i * (double)i);
        iterations = i;
        
        if (fabs(current_sum - prev_sum) < TOLERANCE) {
            break;
        }
        prev_sum = current_sum;
    }
    
    printf("Converged after %d iterations\n", iterations);
    printf("Final converged sum: %.10f\n", current_sum);
    
    return 0;
}