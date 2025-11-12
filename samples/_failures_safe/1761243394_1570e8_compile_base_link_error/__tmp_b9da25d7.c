//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10
#define MAX_INPUT 1000000.0
#define MIN_INPUT 0.1

double compute_series(double x, int max_iter) {
    double sum = 0.0;
    double term = x;
    int n = 1;
    
    while (n <= max_iter && fabs(term) > EPSILON) {
        sum += term;
        n++;
        term = term * x / n;
    }
    
    return sum;
}

double validate_input(const char* input_str) {
    char* endptr;
    double value = strtod(input_str, &endptr);
    
    if (endptr == input_str || *endptr != '\n') {
        return -1.0;
    }
    
    if (value < MIN_INPUT || value > MAX_INPUT) {
        return -1.0;
    }
    
    if (value != value) {
        return -1.0;
    }
    
    return value;
}

int main(void) {
    char input_buffer[256];
    double input_value;
    
    printf("Enter a positive number between %.1f and %.1f: ", MIN_INPUT, MAX_INPUT);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    input_value = validate_input(input_buffer);
    if (input_value < 0.0) {
        fprintf(stderr, "Invalid input: must be a number between %.1f and %.1f\n", MIN_INPUT, MAX_INPUT);
        return EXIT_FAILURE;
    }
    
    double result = compute_series(input_value, MAX_ITERATIONS);
    double reference = exp(input_value) - 1.0;
    
    printf("Computed series sum: %.10f\n", result);
    printf("Reference value (e^x - 1): %.10f\n", reference);
    printf("Absolute difference: %.10f\n", fabs(result - reference));
    
    return EXIT_SUCCESS;
}