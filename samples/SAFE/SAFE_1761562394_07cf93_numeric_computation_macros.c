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
        return NAN;
    }
    
    double result = 0.0;
    double term = 1.0;
    int n = 1;
    
    while (n <= max_iter && fabs(term) > EPSILON) {
        result += term;
        term = term * x / n;
        n++;
        
        if (fabs(result) > 1e100) {
            return INFINITY;
        }
    }
    
    return result;
}

int main(void) {
    double input_value;
    char buffer[100];
    
    printf("Enter a positive number for series computation: ");
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    char *endptr;
    input_value = strtod(buffer, &endptr);
    
    if (endptr == buffer || *endptr != '\n') {
        fprintf(stderr, "Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    if (input_value <= 0.0 || input_value > MAX_INPUT) {
        fprintf(stderr, "Input must be between %g and %g\n", MIN_INPUT, MAX_INPUT);
        return EXIT_FAILURE;
    }
    
    double result = compute_series(input_value, MAX_ITERATIONS);
    
    if (isnan(result)) {
        fprintf(stderr, "Computation resulted in NaN\n");
        return EXIT_FAILURE;
    }
    
    if (isinf(result)) {
        fprintf(stderr, "Computation resulted in infinity\n");
        return EXIT_FAILURE;
    }
    
    printf("Series computation result: %.10f\n", result);
    
    return EXIT_SUCCESS;
}