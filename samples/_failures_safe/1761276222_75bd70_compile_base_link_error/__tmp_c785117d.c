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
    if (x <= 0.0 || x > MAX_INPUT) {
        return 0.0;
    }
    
    double result = 0.0;
    double term = x;
    int n = 1;
    
    while (n <= MAX_ITERATIONS && fabs(term) > TOLERANCE) {
        result += term;
        n++;
        term = -term * x * x / ((2 * n - 1) * (2 * n - 2));
        
        if (fabs(term) > 1e100) {
            break;
        }
    }
    
    return result;
}

double validate_input(const char* input) {
    char* endptr;
    double value = strtod(input, &endptr);
    
    if (endptr == input || *endptr != '\0') {
        return -1.0;
    }
    
    if (value < 0.0 || value > MAX_INPUT) {
        return -1.0;
    }
    
    if (value < MIN_INPUT && value > 0.0) {
        return -1.0;
    }
    
    return value;
}

int main(void) {
    char input_buffer[256];
    double input_value;
    
    printf("Enter a positive number (0 < x <= %g): ", MAX_INPUT);
    
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
    
    if (input_value < 0.0) {
        fprintf(stderr, "Invalid input: must be a positive number between %g and %g\n", MIN_INPUT, MAX_INPUT);
        return EXIT_FAILURE;
    }
    
    double result = compute_series(input_value);
    double reference = sin(input_value);
    
    printf("Series approximation: %.15g\n", result);
    printf("Reference sin(x):    %.15g\n", reference);
    printf("Absolute error:      %.15g\n", fabs(result - reference));
    
    return EXIT_SUCCESS;
}