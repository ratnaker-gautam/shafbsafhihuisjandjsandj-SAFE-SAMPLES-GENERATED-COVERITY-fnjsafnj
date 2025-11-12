//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <float.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10
#define MAX_INPUT 1000000.0
#define MIN_INPUT 1e-10

double safe_power(double base, int exponent) {
    double result = 1.0;
    int abs_exp = exponent < 0 ? -exponent : exponent;
    
    if (base == 0.0 && exponent <= 0) {
        return NAN;
    }
    
    for (int i = 0; i < abs_exp; i++) {
        if (result > DBL_MAX / fabs(base)) {
            return exponent > 0 ? (base > 0 ? DBL_MAX : -DBL_MAX) : 0.0;
        }
        result *= base;
    }
    
    if (exponent < 0) {
        if (fabs(result) < 1.0 / DBL_MAX) {
            return base > 0 ? DBL_MAX : -DBL_MAX;
        }
        result = 1.0 / result;
    }
    
    return result;
}

double compute_series(double x, int n_terms) {
    if (n_terms <= 0 || n_terms > MAX_ITERATIONS) {
        return NAN;
    }
    
    double sum = 0.0;
    double term;
    
    for (int i = 0; i < n_terms; i++) {
        term = safe_power(x, i + 1) / (i + 1.0);
        if (isnan(term) || isinf(term)) {
            return NAN;
        }
        
        if (i % 2 == 0) {
            sum += term;
        } else {
            sum -= term;
        }
        
        if (fabs(term) < EPSILON) {
            break;
        }
    }
    
    return sum;
}

int main(void) {
    double input_value;
    int num_terms;
    
    printf("Enter a value between %.10g and %g: ", MIN_INPUT, MAX_INPUT);
    if (scanf("%lf", &input_value) != 1) {
        fprintf(stderr, "Error: Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    if (input_value < MIN_INPUT || input_value > MAX_INPUT) {
        fprintf(stderr, "Error: Input out of valid range\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter number of terms (1-%d): ", MAX_ITERATIONS);
    if (scanf("%d", &num_terms) != 1) {
        fprintf(stderr, "Error: Invalid term count\n");
        return EXIT_FAILURE;
    }
    
    if (num_terms < 1 || num_terms > MAX_ITERATIONS) {
        fprintf(stderr, "Error: Term count out of valid range\n");
        return EXIT_FAILURE;
    }
    
    double result = compute_series(input_value, num_terms);
    
    if (isnan(result)) {
        fprintf(stderr, "Error: Computation resulted in invalid value\n");
        return EXIT_FAILURE;
    }
    
    if (isinf(result)) {
        fprintf(stderr, "Error: Computation resulted in overflow\n");
        return EXIT_FAILURE;
    }
    
    printf("Series approximation: %.15g\n", result);
    printf("Reference log(1+%.15g): %.15g\n", input_value, log1p(input_value));
    
    return EXIT_SUCCESS;
}