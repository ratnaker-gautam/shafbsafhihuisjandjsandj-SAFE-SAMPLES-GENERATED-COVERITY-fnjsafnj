//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10
#define MAX_INPUT 1000000.0
#define MIN_INPUT 1e-10
#define E_CONST 2.71828182845904523536

double safe_pow(double base, double exponent) {
    if (base < 0.0 && fmod(exponent, 1.0) != 0.0) {
        return NAN;
    }
    if (base == 0.0 && exponent < 0.0) {
        return INFINITY;
    }
    return pow(base, exponent);
}

double compute_series_sum(double x, int terms) {
    if (terms <= 0 || terms > MAX_ITERATIONS) {
        return NAN;
    }
    double sum = 0.0;
    double term = 1.0;
    for (int n = 1; n <= terms; n++) {
        term *= x / n;
        if (!isfinite(term)) {
            return NAN;
        }
        sum += term;
        if (!isfinite(sum)) {
            return NAN;
        }
    }
    return sum;
}

int main(void) {
    double x;
    int terms;
    
    printf("Enter value for x (1e-10 to 1000000): ");
    if (scanf("%lf", &x) != 1) {
        fprintf(stderr, "Error: Invalid input for x\n");
        return EXIT_FAILURE;
    }
    
    if (x < MIN_INPUT || x > MAX_INPUT) {
        fprintf(stderr, "Error: x out of valid range\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter number of terms (1 to 1000): ");
    if (scanf("%d", &terms) != 1) {
        fprintf(stderr, "Error: Invalid input for terms\n");
        return EXIT_FAILURE;
    }
    
    if (terms < 1 || terms > MAX_ITERATIONS) {
        fprintf(stderr, "Error: terms out of valid range\n");
        return EXIT_FAILURE;
    }
    
    double series_result = compute_series_sum(x, terms);
    if (isnan(series_result)) {
        fprintf(stderr, "Error: Series computation failed\n");
        return EXIT_FAILURE;
    }
    
    double exp_result = safe_pow(E_CONST, x);
    if (isnan(exp_result) || !isfinite(exp_result)) {
        fprintf(stderr, "Error: Exponential computation failed\n");
        return EXIT_FAILURE;
    }
    
    double error = fabs(series_result - exp_result);
    if (error > EPSILON) {
        printf("Series approximation: %.10f\n", series_result);
        printf("Exact exponential: %.10f\n", exp_result);
        printf("Absolute error: %.10f\n", error);
    } else {
        printf("Series converged to exponential within tolerance\n");
        printf("Result: %.10f\n", series_result);
    }
    
    return EXIT_SUCCESS;
}