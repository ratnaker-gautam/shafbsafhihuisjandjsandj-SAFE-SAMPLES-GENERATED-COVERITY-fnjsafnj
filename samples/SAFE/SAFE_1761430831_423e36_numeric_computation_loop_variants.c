//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define TOLERANCE 1e-10

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
    double pi_squared_over_6 = 3.14159265358979323846 * 3.14159265358979323846 / 6.0;
    double best_approx = 0.0;
    int best_n = 0;
    
    for (int n = 1; n <= max_terms; n++) {
        double current_sum = compute_series_sum(n);
        double error = fabs(current_sum - pi_squared_over_6);
        
        if (error < TOLERANCE) {
            best_approx = current_sum;
            best_n = n;
            break;
        }
        
        if (n == 1 || error < fabs(best_approx - pi_squared_over_6)) {
            best_approx = current_sum;
            best_n = n;
        }
    }
    
    printf("Best approximation found with n=%d terms\n", best_n);
    printf("Computed sum: %.15f\n", best_approx);
    printf("Expected value: %.15f\n", pi_squared_over_6);
    printf("Absolute error: %.15f\n", fabs(best_approx - pi_squared_over_6));
    
    return best_approx;
}

int main(void) {
    printf("Basel Problem Series Approximation\n");
    printf("Computing sum of 1/n^2 from n=1 to maximum terms\n");
    
    int max_terms;
    printf("Enter maximum number of terms to compute (1-10000): ");
    
    if (scanf("%d", &max_terms) != 1) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    if (max_terms < 1 || max_terms > 10000) {
        fprintf(stderr, "Number of terms must be between 1 and 10000\n");
        return EXIT_FAILURE;
    }
    
    double result = compute_approximation(max_terms);
    
    int convergence_check = 0;
    double prev_sum = 0.0;
    for (int n = 1; n <= 10 && n <= max_terms; n++) {
        double current_sum = compute_series_sum(n);
        if (n > 1) {
            double diff = fabs(current_sum - prev_sum);
            if (diff < 1e-15) {
                convergence_check++;
            }
        }
        prev_sum = current_sum;
    }
    
    if (convergence_check >= 3) {
        printf("Series appears to be converging rapidly\n");
    } else {
        printf("Series convergence requires more terms\n");
    }
    
    int n = 1;
    double running_sum = 0.0;
    while (n <= 5 && n <= max_terms) {
        if (n == 0) {
            fprintf(stderr, "Division by zero detected\n");
            return EXIT_FAILURE;
        }
        double term = 1.0 / ((double)n * (double)n);
        running_sum += term;
        printf("After %d terms: partial sum = %.10f\n", n, running_sum);
        n++;
    }
    
    return EXIT_SUCCESS;
}