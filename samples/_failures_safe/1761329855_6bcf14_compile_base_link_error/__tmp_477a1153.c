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
        double numerator = safe_pow(-1, n + 1);
        double denominator = n * safe_pow(1 + x, n);
        if (isnan(numerator) || isnan(denominator) || denominator == 0.0) {
            return NAN;
        }
        term = numerator / denominator;
        if (isinf(term) || isnan(term)) {
            return NAN;
        }
        sum += term;
        if (isinf(sum) || isnan(sum)) {
            return NAN;
        }
    }
    return sum;
}

int main() {
    double x;
    int terms;
    
    printf("Enter value for x (0.01 to 1000000): ");
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
    
    double result = compute_series_sum(x, terms);
    
    if (isnan(result)) {
        fprintf(stderr, "Error: Computation resulted in invalid value\n");
        return EXIT_FAILURE;
    }
    
    if (isinf(result)) {
        fprintf(stderr, "Error: Computation resulted in infinity\n");
        return EXIT_FAILURE;
    }
    
    printf("Series sum: %.10f\n", result);
    printf("Theoretical limit: ln(%.10f) = %.10f\n", 1.0 + x, log(1.0 + x));
    
    return EXIT_SUCCESS;
}