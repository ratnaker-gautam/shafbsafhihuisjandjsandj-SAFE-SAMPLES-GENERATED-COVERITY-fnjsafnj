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
        return -1.0;
    }
    
    double result = 0.0;
    double term = x;
    int n = 1;
    
    while (n <= max_iter && fabs(term) > EPSILON) {
        result += term;
        if (n % 2 == 0) {
            term = (term * x * x) / ((2 * n) * (2 * n + 1));
        } else {
            term = (-term * x * x) / ((2 * n) * (2 * n + 1));
        }
        n++;
        
        if (fabs(term) > 1e100 || result > 1e100) {
            return -1.0;
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
    
    if (value <= 0.0 || value > MAX_INPUT) {
        return -1.0;
    }
    
    return value;
}

int main() {
    char input_buffer[256];
    
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
    
    double x = validate_input(input_buffer);
    if (x < 0.0) {
        fprintf(stderr, "Invalid input: must be positive number between 1e-10 and 1e6\n");
        return EXIT_FAILURE;
    }
    
    double result = compute_series(x, MAX_ITERATIONS);
    if (result < 0.0) {
        fprintf(stderr, "Computation failed: input out of range or series diverged\n");
        return EXIT_FAILURE;
    }
    
    double reference = sin(x) / x;
    printf("Series result: %.10f\n", result);
    printf("Reference (sin(x)/x): %.10f\n", reference);
    printf("Absolute difference: %.10f\n", fabs(result - reference));
    
    return EXIT_SUCCESS;
}