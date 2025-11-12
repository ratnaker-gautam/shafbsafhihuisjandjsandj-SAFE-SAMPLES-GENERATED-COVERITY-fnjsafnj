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
#define MIN_INPUT 0.0001

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

double compute_series(double x, int terms) {
    if (terms <= 0 || terms > MAX_ITERATIONS) return 0.0;
    
    double sum = 0.0;
    double term;
    
    for (int n = 1; n <= terms; n++) {
        term = safe_power(x, n) / n;
        if (n % 2 == 0) term = -term;
        
        if (fabs(term) < EPSILON) break;
        if (sum + term == sum) break;
        
        sum += term;
    }
    
    return sum;
}

int main(void) {
    double input_value;
    int num_terms;
    
    printf("Enter a value between %g and %g: ", MIN_INPUT, MAX_INPUT);
    if (scanf("%lf", &input_value) != 1) {
        printf("Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    if (input_value < MIN_INPUT || input_value > MAX_INPUT) {
        printf("Input out of valid range\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter number of terms (1-%d): ", MAX_ITERATIONS);
    if (scanf("%d", &num_terms) != 1) {
        printf("Invalid term count format\n");
        return EXIT_FAILURE;
    }
    
    if (num_terms < 1 || num_terms > MAX_ITERATIONS) {
        printf("Term count out of valid range\n");
        return EXIT_FAILURE;
    }
    
    double result = compute_series(input_value, num_terms);
    double reference = log(1.0 + input_value);
    
    printf("Series approximation: %.10f\n", result);
    printf("Reference log(1+x):  %.10f\n", reference);
    printf("Absolute difference:  %.10f\n", fabs(result - reference));
    
    return EXIT_SUCCESS;
}