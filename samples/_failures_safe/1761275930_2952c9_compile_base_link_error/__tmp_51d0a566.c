//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000
#define TOLERANCE 1e-10
#define MAX_INPUT 1000000.0
#define MIN_INPUT 1e-10

double compute_series(double x, int max_iter) {
    if (x <= 0.0 || x > MAX_INPUT) {
        return NAN;
    }
    
    double result = 0.0;
    double term = 1.0;
    int n = 1;
    
    while (n <= max_iter && fabs(term) > TOLERANCE) {
        result += term;
        term = -term * x / n;
        n++;
        
        if (!isfinite(result) || !isfinite(term)) {
            return NAN;
        }
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
    
    if (!isfinite(val)) {
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
        fprintf(stderr, "Error: Computation failed or did not converge\n");
        return EXIT_FAILURE;
    }
    
    double reference = exp(-input);
    double error = fabs(result - reference);
    
    printf("Input: %g\n", input);
    printf("Series result: %.15g\n", result);
    printf("Reference (e^-x): %.15g\n", reference);
    printf("Absolute error: %.15g\n", error);
    
    return EXIT_SUCCESS;
}