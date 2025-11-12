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
    double term = x;
    int n = 1;
    
    while (n <= max_iter && fabs(term) > TOLERANCE) {
        result += term;
        term = -term * x * x / ((2 * n) * (2 * n + 1));
        n++;
        
        if (!isfinite(term)) {
            return NAN;
        }
    }
    
    return result;
}

double validate_input(const char* input_str) {
    char* endptr;
    double value = strtod(input_str, &endptr);
    
    if (endptr == input_str || *endptr != '\0') {
        return NAN;
    }
    
    if (value <= 0.0 || value > MAX_INPUT) {
        return NAN;
    }
    
    if (!isfinite(value)) {
        return NAN;
    }
    
    return value;
}

int main(void) {
    char input_buffer[256];
    double input_value;
    
    printf("Enter a positive number (1e-10 to 1e6): ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = 0;
    while (len < sizeof(input_buffer) && input_buffer[len] != '\0') {
        len++;
    }
    
    if (len > 0 && input_buffer[len-1] == '\n') {
        input_buffer[len-1] = '\0';
    } else if (len == sizeof(input_buffer) - 1) {
        fprintf(stderr, "Input too long\n");
        return EXIT_FAILURE;
    }
    
    input_value = validate_input(input_buffer);
    
    if (isnan(input_value)) {
        fprintf(stderr, "Invalid input: must be a positive number between 1e-10 and 1e6\n");
        return EXIT_FAILURE;
    }
    
    double result = compute_series(input_value, MAX_ITERATIONS);
    
    if (isnan(result)) {
        fprintf(stderr, "Computation failed: series did not converge\n");
        return EXIT_FAILURE;
    }
    
    double reference = sin(input_value) / input_value;
    
    printf("Input value: %.10g\n", input_value);
    printf("Computed result: %.15g\n", result);
    printf("Reference (sinc): %.15g\n", reference);
    printf("Absolute error: %.15g\n", fabs(result - reference));
    
    return EXIT_SUCCESS;
}