//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10
#define MAX_INPUT 1000000.0
#define MIN_INPUT 1e-10

double compute_series(double x, int max_iter) {
    if (x <= 0.0 || x > MAX_INPUT) {
        return 0.0;
    }
    
    double result = 0.0;
    double term = x;
    int n = 1;
    
    while (n <= max_iter && fabs(term) > EPSILON) {
        result += term;
        n++;
        term = term * x / n;
        
        if (fabs(result) > 1e100) {
            break;
        }
    }
    
    return result;
}

double validate_input(const char* input_str) {
    char* endptr;
    double value = strtod(input_str, &endptr);
    
    if (endptr == input_str || *endptr != '\0') {
        return -1.0;
    }
    
    if (value <= 0.0 || value > MAX_INPUT) {
        return -1.0;
    }
    
    if (value < MIN_INPUT) {
        return -1.0;
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
    }
    
    input_value = validate_input(input_buffer);
    
    if (input_value < 0.0) {
        fprintf(stderr, "Invalid input: must be a positive number between 1e-10 and 1e6\n");
        return EXIT_FAILURE;
    }
    
    double result = compute_series(input_value, MAX_ITERATIONS);
    
    if (isnan(result) || isinf(result)) {
        fprintf(stderr, "Computation resulted in invalid value\n");
        return EXIT_FAILURE;
    }
    
    printf("Series computation result: %.10f\n", result);
    printf("Reference exponential: %.10f\n", exp(input_value));
    
    return EXIT_SUCCESS;
}