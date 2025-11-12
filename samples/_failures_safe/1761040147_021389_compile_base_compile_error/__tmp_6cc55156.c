//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: numeric_computation
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

double compute_series(double x, int max_iter) {
    if (x <= 0.0 || x > MAX_INPUT) {
        return NAN;
    }
    
    double result = 0.0;
    double term = x;
    int n = 1;
    
    while (n <= max_iter && fabs(term) > EPSILON) {
        result += term;
        if (n >= max_iter) break;
        
        double next_term = term * x / (n + 1);
        if (fabs(next_term) > DBL_MAX / 2.0) {
            break;
        }
        term = next_term;
        n++;
    }
    
    return result;
}

double validate_input(const char* str) {
    char* endptr;
    double val = strtod(str, &endptr);
    
    if (endptr == str || *endptr != '\0') {
        return NAN;
    }
    
    if (val <= 0.0 || val > MAX_INPUT) {
        return NAN;
    }
    
    return val;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <positive_number>\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    double input = validate_input(argv[1]);
    if (isnan(input)) {
        fprintf(stderr, "Error: Invalid input. Must be a positive number between %g and %g\n", MIN_INPUT, MAX_INPUT);
        return EXIT_FAILURE;
    }
    
    double result = compute_series(input, MAX_ITERATIONS);
    if (isnan(result)) {
        fprintf(stderr, "Error: Computation failed for input %g\n", input);
        return EXIT_FAILURE;
    }
    
    double reference = exp(input) - 1.0;
    double error = fabs(result - reference);
    
    printf("Input: %.10g\n", input);
    printf("Series result: %.15g\n", result);
    printf("Reference (e^x - 1): %.15g\n", reference);
    printf("Absolute error: %.15g\n", error);
    
    return EXIT_SUCCESS;
}