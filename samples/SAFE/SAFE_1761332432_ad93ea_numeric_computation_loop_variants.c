//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10

double compute_series_sum(int n) {
    double sum = 0.0;
    for (int i = 1; i <= n; i++) {
        if (i == 0) {
            fprintf(stderr, "Division by zero detected\n");
            exit(EXIT_FAILURE);
        }
        double term = 1.0 / ((double)i * (double)i);
        if (isinf(term) || isnan(term)) {
            fprintf(stderr, "Numerical overflow detected\n");
            exit(EXIT_FAILURE);
        }
        sum += term;
        if (isinf(sum) || isnan(sum)) {
            fprintf(stderr, "Numerical overflow in sum\n");
            exit(EXIT_FAILURE);
        }
    }
    return sum;
}

double compute_approximation(int max_terms) {
    double pi_value = acos(-1.0);
    double pi_squared_over_6 = pi_value * pi_value / 6.0;
    double best_approx = 0.0;
    int best_n = 0;
    
    for (int n = 1; n <= max_terms; n++) {
        double current_sum = compute_series_sum(n);
        double error = fabs(current_sum - pi_squared_over_6);
        
        if (n == 1 || error < best_approx) {
            best_approx = error;
            best_n = n;
        }
        
        if (error < EPSILON) {
            break;
        }
    }
    
    return compute_series_sum(best_n);
}

int main() {
    int max_terms;
    
    printf("Enter maximum number of terms to compute (1-1000): ");
    if (scanf("%d", &max_terms) != 1) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    if (max_terms < 1 || max_terms > MAX_ITERATIONS) {
        fprintf(stderr, "Number of terms must be between 1 and 1000\n");
        return EXIT_FAILURE;
    }
    
    double result = compute_approximation(max_terms);
    double pi_value = acos(-1.0);
    double exact_value = pi_value * pi_value / 6.0;
    double error = fabs(result - exact_value);
    
    printf("Computed sum: %.10f\n", result);
    printf("Exact value (pi^2/6): %.10f\n", exact_value);
    printf("Absolute error: %.10f\n", error);
    printf("Relative error: %.10f%%\n", (error / exact_value) * 100.0);
    
    return EXIT_SUCCESS;
}