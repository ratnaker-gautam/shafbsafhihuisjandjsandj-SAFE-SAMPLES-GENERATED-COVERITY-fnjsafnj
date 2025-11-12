//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define TOLERANCE 1e-10
#define M_PI 3.14159265358979323846

double compute_series_sum(int n) {
    if (n <= 0) return 0.0;
    double sum = 0.0;
    for (int i = 1; i <= n; i++) {
        if (i == 0) continue;
        double denominator = (double)i * (double)i;
        if (denominator == 0.0) continue;
        double term = 1.0 / denominator;
        sum += term;
    }
    return sum;
}

double compute_series_sum_while(int n) {
    if (n <= 0) return 0.0;
    double sum = 0.0;
    int i = 1;
    while (i <= n) {
        if (i == 0) {
            i++;
            continue;
        }
        double denominator = (double)i * (double)i;
        if (denominator == 0.0) {
            i++;
            continue;
        }
        double term = 1.0 / denominator;
        sum += term;
        i++;
    }
    return sum;
}

double compute_series_sum_do_while(int n) {
    if (n <= 0) return 0.0;
    double sum = 0.0;
    int i = 1;
    do {
        if (i == 0) {
            i++;
            continue;
        }
        double denominator = (double)i * (double)i;
        if (denominator == 0.0) {
            i++;
            continue;
        }
        double term = 1.0 / denominator;
        sum += term;
        i++;
    } while (i <= n);
    return sum;
}

int main() {
    int n;
    printf("Enter number of terms to compute (1-100000): ");
    
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (n < 1 || n > 100000) {
        fprintf(stderr, "Error: Number must be between 1 and 100000\n");
        return 1;
    }
    
    printf("Computing sum of 1/k^2 from k=1 to %d using different loops:\n", n);
    
    double result_for = compute_series_sum(n);
    double result_while = compute_series_sum_while(n);
    double result_do_while = compute_series_sum_do_while(n);
    
    printf("For loop result:    %.15f\n", result_for);
    printf("While loop result:  %.15f\n", result_while);
    printf("Do-while result:    %.15f\n", result_do_while);
    
    double diff1 = fabs(result_for - result_while);
    double diff2 = fabs(result_for - result_do_while);
    
    if (diff1 < TOLERANCE && diff2 < TOLERANCE) {
        printf("All methods agree within tolerance\n");
    } else {
        printf("Warning: Methods differ beyond tolerance\n");
    }
    
    double pi_estimate = sqrt(6.0 * result_for);
    printf("Pi estimate from series: %.15f\n", pi_estimate);
    printf("Actual Pi:               %.15f\n", M_PI);
    printf("Difference:              %.15f\n", fabs(pi_estimate - M_PI));
    
    return 0;
}