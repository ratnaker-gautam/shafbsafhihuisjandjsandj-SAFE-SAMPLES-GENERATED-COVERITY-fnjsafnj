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

int main(void) {
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
    
    double result_for = compute_series_sum(n);
    double result_while = compute_series_sum_while(n);
    double result_do_while = compute_series_sum_do_while(n);
    
    printf("For loop result: %.15f\n", result_for);
    printf("While loop result: %.15f\n", result_while);
    printf("Do-while loop result: %.15f\n", result_do_while);
    
    double diff1 = fabs(result_for - result_while);
    double diff2 = fabs(result_while - result_do_while);
    
    if (diff1 < TOLERANCE && diff2 < TOLERANCE) {
        printf("All methods agree within tolerance\n");
    } else {
        printf("Warning: Methods differ beyond tolerance\n");
    }
    
    double pi_estimate = sqrt(result_for * 6.0);
    double pi_actual = 3.14159265358979323846;
    printf("Pi estimate: %.15f\n", pi_estimate);
    printf("Pi actual:   %.15f\n", pi_actual);
    printf("Difference:  %.15f\n", fabs(pi_estimate - pi_actual));
    
    return 0;
}