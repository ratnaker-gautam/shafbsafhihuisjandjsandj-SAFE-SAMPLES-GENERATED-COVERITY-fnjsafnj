//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define TOLERANCE 1e-10
#define MAX_INPUT 1e6
#define MIN_INPUT 1e-6

double compute_series(double x) {
    if (x <= MIN_INPUT || x >= MAX_INPUT) {
        return NAN;
    }
    
    double result = 0.0;
    double term = x;
    int n = 1;
    
    while (n <= MAX_ITERATIONS && fabs(term) > TOLERANCE) {
        result += term;
        term = -term * x * x / ((2 * n) * (2 * n + 1));
        n++;
        
        if (!isfinite(term)) {
            return NAN;
        }
    }
    
    return result;
}

double validate_input(const char* str) {
    if (str == NULL) {
        return NAN;
    }
    
    char* endptr;
    double value = strtod(str, &endptr);
    
    if (endptr == str || *endptr != '\0') {
        return NAN;
    }
    
    if (!isfinite(value)) {
        return NAN;
    }
    
    return value;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number>\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    double input = validate_input(argv[1]);
    if (isnan(input)) {
        fprintf(stderr, "Error: Invalid input. Please provide a valid finite number.\n");
        return EXIT_FAILURE;
    }
    
    double result = compute_series(input);
    if (isnan(result)) {
        fprintf(stderr, "Error: Computation failed. Input may be out of valid range.\n");
        return EXIT_FAILURE;
    }
    
    double reference = sin(input);
    double error = fabs(result - reference);
    
    printf("Input: %.6f\n", input);
    printf("Computed result: %.10f\n", result);
    printf("Reference sin(x): %.10f\n", reference);
    printf("Absolute error: %.2e\n", error);
    
    return EXIT_SUCCESS;
}