//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define MAX_ITERATIONS 1000
#define TOLERANCE 1e-10
#define MAX_INPUT 1000000.0
#define MIN_INPUT 0.0001

double compute_series(double x, int max_iter) {
    double sum = 0.0;
    double term = x;
    int n = 1;
    
    while (n <= max_iter && fabs(term) > TOLERANCE) {
        sum += term;
        term = -term * x * x / ((2 * n) * (2 * n + 1));
        n++;
        
        if (fabs(term) > 1e100 || isnan(term)) {
            break;
        }
    }
    return sum;
}

double validate_input(const char* input_str) {
    char* endptr;
    double value = strtod(input_str, &endptr);
    
    if (endptr == input_str || *endptr != '\0') {
        return NAN;
    }
    
    if (value < MIN_INPUT || value > MAX_INPUT) {
        return NAN;
    }
    
    if (isnan(value) || isinf(value)) {
        return NAN;
    }
    
    return value;
}

int main(void) {
    char input_buffer[256];
    double input_value;
    
    printf("Enter a value between %g and %g: ", MIN_INPUT, MAX_INPUT);
    
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
    }
    
    input_value = validate_input(input_buffer);
    
    if (isnan(input_value)) {
        fprintf(stderr, "Invalid input: must be a number between %g and %g\n", MIN_INPUT, MAX_INPUT);
        return EXIT_FAILURE;
    }
    
    double result = compute_series(input_value, MAX_ITERATIONS);
    double reference = sin(input_value);
    
    if (isnan(result) || isinf(result)) {
        fprintf(stderr, "Numerical instability detected\n");
        return EXIT_FAILURE;
    }
    
    printf("Series approximation: %.15g\n", result);
    printf("Reference sin(x):    %.15g\n", reference);
    printf("Absolute error:      %.15g\n", fabs(result - reference));
    
    return EXIT_SUCCESS;
}