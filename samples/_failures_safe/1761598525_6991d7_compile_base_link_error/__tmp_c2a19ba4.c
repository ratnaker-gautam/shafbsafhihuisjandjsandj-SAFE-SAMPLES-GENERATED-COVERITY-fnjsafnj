//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define MAX_ITERATIONS 1000
#define TOLERANCE 1e-10
#define MAX_INPUT 1000000.0
#define MIN_INPUT 1e-10

double compute_series(double x) {
    if (x < MIN_INPUT || x > MAX_INPUT) {
        return NAN;
    }
    
    double result = 0.0;
    double term = x;
    int n = 1;
    
    while (n <= MAX_ITERATIONS && fabs(term) > TOLERANCE) {
        result += term;
        term = -term * x * x / ((2 * n) * (2 * n + 1));
        n++;
        
        if (isinf(result) || isnan(result)) {
            return NAN;
        }
    }
    
    return result;
}

double validate_input(const char* input) {
    char* endptr;
    double value = strtod(input, &endptr);
    
    if (endptr == input || *endptr != '\n' && *endptr != '\0') {
        return NAN;
    }
    
    if (value < MIN_INPUT || value > MAX_INPUT) {
        return NAN;
    }
    
    return value;
}

int main(void) {
    char input_buffer[256];
    
    printf("Enter a positive number between %.2e and %.2e: ", MIN_INPUT, MAX_INPUT);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    double x = validate_input(input_buffer);
    
    if (isnan(x)) {
        fprintf(stderr, "Invalid input: must be a number between %.2e and %.2e\n", MIN_INPUT, MAX_INPUT);
        return EXIT_FAILURE;
    }
    
    double result = compute_series(x);
    
    if (isnan(result)) {
        fprintf(stderr, "Computation failed: numerical instability detected\n");
        return EXIT_FAILURE;
    }
    
    double reference = sin(x) / x;
    
    printf("Computed series result: %.15e\n", result);
    printf("Reference value (sin(x)/x): %.15e\n", reference);
    printf("Absolute error: %.15e\n", fabs(result - reference));
    
    return EXIT_SUCCESS;
}