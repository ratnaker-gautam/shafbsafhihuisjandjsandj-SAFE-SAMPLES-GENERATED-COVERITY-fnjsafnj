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
    double term = 1.0;
    int n = 1;
    
    while (n <= max_iter && fabs(term) > EPSILON) {
        result += term;
        term = term * (-x) / n;
        n++;
        
        if (fabs(result) > 1e100) {
            return -1.0;
        }
    }
    
    return result;
}

double validate_input(const char* str) {
    char* endptr;
    double val = strtod(str, &endptr);
    
    if (endptr == str || *endptr != '\0') {
        return -1.0;
    }
    
    if (val < MIN_INPUT || val > MAX_INPUT) {
        return -1.0;
    }
    
    if (val != val) {
        return -1.0;
    }
    
    return val;
}

int main(void) {
    char input[256];
    double x;
    
    printf("Enter a positive number (1e-10 to 1000000): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = 0;
    while (len < sizeof(input) && input[len] != '\0') {
        len++;
    }
    
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    } else if (len == sizeof(input) - 1) {
        fprintf(stderr, "Input too long\n");
        return EXIT_FAILURE;
    }
    
    x = validate_input(input);
    if (x < 0.0) {
        fprintf(stderr, "Invalid input: must be positive number between 1e-10 and 1000000\n");
        return EXIT_FAILURE;
    }
    
    double result = compute_series(x, MAX_ITERATIONS);
    if (result < 0.0) {
        fprintf(stderr, "Computation failed: input out of range or series diverged\n");
        return EXIT_FAILURE;
    }
    
    double reference = exp(-x);
    double error = fabs(result - reference);
    
    printf("Series approximation: %.10g\n", result);
    printf("Reference value (e^-x): %.10g\n", reference);
    printf("Absolute error: %.10g\n", error);
    
    return EXIT_SUCCESS;
}