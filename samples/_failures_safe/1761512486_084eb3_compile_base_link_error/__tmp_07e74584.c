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
    if (x < MIN_INPUT || x > MAX_INPUT) {
        return NAN;
    }
    
    double result = 0.0;
    double term = x;
    int n = 1;
    
    while (n <= MAX_ITERATIONS && fabs(term) > TOLERANCE) {
        result += term;
        term = -term * x * x / ((2 * n) * (2 * n + 1));
        n++;
        
        if (!isfinite(result) || !isfinite(term)) {
            return NAN;
        }
    }
    
    return result;
}

int main(void) {
    double input;
    char buffer[100];
    
    printf("Enter a positive number for series computation: ");
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    char *endptr;
    input = strtod(buffer, &endptr);
    
    if (endptr == buffer || *endptr != '\n') {
        fprintf(stderr, "Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    if (input <= 0.0) {
        fprintf(stderr, "Input must be positive\n");
        return EXIT_FAILURE;
    }
    
    double result = compute_series(input);
    
    if (isnan(result)) {
        fprintf(stderr, "Computation failed or input out of range\n");
        return EXIT_FAILURE;
    }
    
    printf("Series result: %.15f\n", result);
    printf("Reference sin(x): %.15f\n", sin(input));
    
    return EXIT_SUCCESS;
}