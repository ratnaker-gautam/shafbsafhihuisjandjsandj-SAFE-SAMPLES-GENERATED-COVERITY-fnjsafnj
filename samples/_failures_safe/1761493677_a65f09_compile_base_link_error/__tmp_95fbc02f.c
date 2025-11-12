//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10

double compute_series_sum(int n) {
    double sum = 0.0;
    for (int i = 1; i <= n; i++) {
        double term = 1.0 / (i * i);
        if (isinf(term) || isnan(term)) {
            return -1.0;
        }
        sum += term;
    }
    return sum;
}

double compute_series_sum_while(int n) {
    double sum = 0.0;
    int i = 1;
    while (i <= n) {
        double term = 1.0 / (i * i);
        if (isinf(term) || isnan(term)) {
            return -1.0;
        }
        sum += term;
        i++;
    }
    return sum;
}

double compute_series_sum_do_while(int n) {
    if (n <= 0) {
        return 0.0;
    }
    double sum = 0.0;
    int i = 1;
    do {
        double term = 1.0 / (i * i);
        if (isinf(term) || isnan(term)) {
            return -1.0;
        }
        sum += term;
        i++;
    } while (i <= n);
    return sum;
}

int main() {
    int n;
    printf("Enter number of terms to compute (1-10000): ");
    
    if (scanf("%d", &n) != 1) {
        printf("Invalid input. Please enter an integer.\n");
        return 1;
    }
    
    if (n < 1 || n > 10000) {
        printf("Input out of range. Please enter a number between 1 and 10000.\n");
        return 1;
    }
    
    double result_for = compute_series_sum(n);
    double result_while = compute_series_sum_while(n);
    double result_do_while = compute_series_sum_do_while(n);
    
    if (result_for < 0 || result_while < 0 || result_do_while < 0) {
        printf("Numerical error occurred during computation.\n");
        return 1;
    }
    
    printf("Series sum using for loop: %.10f\n", result_for);
    printf("Series sum using while loop: %.10f\n", result_while);
    printf("Series sum using do-while loop: %.10f\n", result_do_while);
    
    double diff1 = fabs(result_for - result_while);
    double diff2 = fabs(result_for - result_do_while);
    
    if (diff1 < EPSILON && diff2 < EPSILON) {
        printf("All loop variants produced consistent results.\n");
    } else {
        printf("Warning: Loop variants produced different results.\n");
    }
    
    double pi_estimate = sqrt(6.0 * result_for);
    printf("Estimated value of pi: %.10f\n", pi_estimate);
    
    return 0;
}