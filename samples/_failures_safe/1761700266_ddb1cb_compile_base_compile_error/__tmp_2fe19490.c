//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>
#include <float.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10
#define MAX_INPUT 1000000.0
#define MIN_INPUT 1e-10

double safe_power(double base, int exponent) {
    if (exponent == 0) return 1.0;
    if (base == 0.0) return 0.0;
    
    double result = 1.0;
    int abs_exp = exponent < 0 ? -exponent : exponent;
    
    for (int i = 0; i < abs_exp; i++) {
        if (result > DBL_MAX / fabs(base)) {
            return exponent > 0 ? DBL_MAX : 0.0;
        }
        result *= base;
    }
    
    return exponent < 0 ? 1.0 / result : result;
}

double compute_series(double x, int n_terms) {
    if (n_terms <= 0 || n_terms > MAX_ITERATIONS) {
        return 0.0;
    }
    
    double sum = 0.0;
    double term;
    
    for (int i = 0; i < n_terms; i++) {
        term = safe_power(x, i) / (i + 1.0);
        if (fabs(term) < EPSILON) break;
        
        if (i % 2 == 0) {
            sum += term;
        } else {
            sum -= term;
        }
        
        if (isinf(sum) || isnan(sum)) {
            return 0.0;
        }
    }
    
    return sum;
}

int main(void) {
    double input_value;
    int terms;
    
    printf("Enter a value between %.10g and %.10g: ", MIN_INPUT, MAX_INPUT);
    if (scanf("%lf", &input_value) != 1) {
        printf("Invalid input format.\n");
        return EXIT_FAILURE;
    }
    
    if (input_value < MIN_INPUT || input_value > MAX_INPUT) {
        printf("Input out of valid range.\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter number of terms (1-%d): ", MAX_ITERATIONS);
    if (scanf("%d", &terms) != 1) {
        printf("Invalid term count.\n");
        return EXIT_FAILURE;
    }
    
    if (terms < 1 || terms > MAX_ITERATIONS) {
        printf("Term count out of valid range.\n");
        return EXIT_FAILURE;
    }
    
    double result = compute_series(input_value, terms);
    
    if (isnan(result) || isinf(result)) {
        printf("Computation resulted in invalid value.\n");
        return EXIT_FAILURE;
    }
    
    printf("Series approximation: %.15g\n", result);
    printf("Reference value (ln(1+x)): %.15g\n", log1p(input_value));
    printf("Absolute difference: %.15g\n", fabs(result - log1p(input_value)));
    
    return EXIT_SUCCESS;
}