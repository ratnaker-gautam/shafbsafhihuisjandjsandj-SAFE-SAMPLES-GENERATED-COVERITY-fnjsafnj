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
    double term = x;
    int n = 1;
    
    while (n <= max_iter && fabs(term) > EPSILON) {
        if (n % 2 == 0) {
            result -= term;
        } else {
            result += term;
        }
        
        n++;
        term = term * x / n;
        
        if (!isfinite(term)) {
            return NAN;
        }
    }
    
    return result;
}

int main(void) {
    double input_value;
    char buffer[256];
    
    printf("Enter a positive number (1e-10 to 1e6): ");
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    if (sscanf(buffer, "%lf", &input_value) != 1) {
        fprintf(stderr, "Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    if (input_value <= MIN_INPUT || input_value > MAX_INPUT) {
        fprintf(stderr, "Input out of valid range\n");
        return EXIT_FAILURE;
    }
    
    double result = compute_series(input_value, MAX_ITERATIONS);
    
    if (isnan(result)) {
        fprintf(stderr, "Computation failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Series result: %.10f\n", result);
    printf("Reference value (1 - exp(-x)): %.10f\n", 1.0 - exp(-input_value));
    
    return EXIT_SUCCESS;
}