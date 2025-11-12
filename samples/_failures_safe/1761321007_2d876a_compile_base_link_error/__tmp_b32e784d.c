//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10
#define MAX_INPUT 1000000.0
#define MIN_INPUT 0.0001

double compute_series(double x, int max_iter) {
    double sum = 0.0;
    double term = x;
    int n = 1;
    
    while (n <= max_iter && fabs(term) > EPSILON) {
        sum += term;
        term = -term * x * x / ((2 * n) * (2 * n + 1));
        n++;
        
        if (fabs(sum) > MAX_INPUT) {
            return NAN;
        }
    }
    
    return sum;
}

double validate_input(const char* input) {
    char* endptr;
    double value = strtod(input, &endptr);
    
    if (endptr == input || *endptr != '\0') {
        return NAN;
    }
    
    if (value < -MAX_INPUT || value > MAX_INPUT) {
        return NAN;
    }
    
    if (fabs(value) < MIN_INPUT && value != 0.0) {
        return NAN;
    }
    
    return value;
}

int main(void) {
    char input_buffer[256];
    double x;
    
    printf("Enter a value for x (between -%g and %g): ", MAX_INPUT, MAX_INPUT);
    
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
    
    x = validate_input(input_buffer);
    
    if (isnan(x)) {
        fprintf(stderr, "Invalid input: must be a number between -%g and %g\n", MAX_INPUT, MAX_INPUT);
        return EXIT_FAILURE;
    }
    
    double result = compute_series(x, MAX_ITERATIONS);
    
    if (isnan(result)) {
        fprintf(stderr, "Computation failed: series diverges or exceeds limits\n");
        return EXIT_FAILURE;
    }
    
    double reference = sin(x);
    double error = fabs(result - reference);
    
    printf("Series approximation: %.10g\n", result);
    printf("Library sin(x): %.10g\n", reference);
    printf("Absolute error: %.10g\n", error);
    
    return EXIT_SUCCESS;
}