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

double compute_series(double x, int max_iter) {
    if (x <= 0.0 || x > MAX_INPUT) {
        return NAN;
    }
    
    double result = 0.0;
    double term = 1.0;
    int n = 1;
    
    while (n <= max_iter && fabs(term) > EPSILON) {
        result += term;
        term = term * x / n;
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
    
    if (endptr == input || *endptr != '\0') {
        return NAN;
    }
    
    if (value <= 0.0 || value > MAX_INPUT) {
        return NAN;
    }
    
    return value;
}

int main(void) {
    char input_buffer[256];
    
    printf("Enter a positive number (1e-10 to 1e6): ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = 0;
    while (len < sizeof(input_buffer) && input_buffer[len] != '\0') {
        if (input_buffer[len] == '\n') {
            input_buffer[len] = '\0';
            break;
        }
        len++;
    }
    
    if (len == sizeof(input_buffer) - 1 && input_buffer[len - 1] != '\n') {
        fprintf(stderr, "Input too long\n");
        return EXIT_FAILURE;
    }
    
    double x = validate_input(input_buffer);
    if (isnan(x)) {
        fprintf(stderr, "Invalid input: must be a positive number between 1e-10 and 1e6\n");
        return EXIT_FAILURE;
    }
    
    double result = compute_series(x, MAX_ITERATIONS);
    
    if (isnan(result)) {
        fprintf(stderr, "Computation failed: result is not finite\n");
        return EXIT_FAILURE;
    }
    
    printf("Series approximation: %.10f\n", result);
    printf("Reference exp(x): %.10f\n", exp(x));
    printf("Absolute error: %.10f\n", fabs(result - exp(x)));
    
    return EXIT_SUCCESS;
}