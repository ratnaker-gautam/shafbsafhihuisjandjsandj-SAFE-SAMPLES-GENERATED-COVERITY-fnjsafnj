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
            fprintf(stderr, "Numerical overflow detected\n");
            exit(EXIT_FAILURE);
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
            fprintf(stderr, "Numerical overflow detected\n");
            exit(EXIT_FAILURE);
        }
        sum += term;
        i++;
    }
    return sum;
}

double compute_series_sum_dowhile(int n) {
    double sum = 0.0;
    int i = 1;
    if (n < 1) {
        return 0.0;
    }
    do {
        double term = 1.0 / (i * i);
        if (isinf(term) || isnan(term)) {
            fprintf(stderr, "Numerical overflow detected\n");
            exit(EXIT_FAILURE);
        }
        sum += term;
        i++;
    } while (i <= n);
    return sum;
}

int main(void) {
    int n;
    printf("Enter number of terms to compute (1-1000): ");
    
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    if (n < 1 || n > MAX_ITERATIONS) {
        fprintf(stderr, "Number of terms must be between 1 and %d\n", MAX_ITERATIONS);
        return EXIT_FAILURE;
    }
    
    double result_for = compute_series_sum(n);
    double result_while = compute_series_sum_while(n);
    double result_dowhile = compute_series_sum_dowhile(n);
    
    printf("For loop result: %.10f\n", result_for);
    printf("While loop result: %.10f\n", result_while);
    printf("Do-while loop result: %.10f\n", result_dowhile);
    
    double diff1 = fabs(result_for - result_while);
    double diff2 = fabs(result_while - result_dowhile);
    
    if (diff1 < EPSILON && diff2 < EPSILON) {
        printf("All methods agree within tolerance\n");
    } else {
        printf("Warning: Methods disagree beyond tolerance\n");
    }
    
    return EXIT_SUCCESS;
}