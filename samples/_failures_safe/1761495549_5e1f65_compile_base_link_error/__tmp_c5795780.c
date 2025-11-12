//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10
#define MAX_INPUT 1000000.0
#define MIN_INPUT 1e-10

double compute_series_sum(double x, int max_iter) {
    if (x <= 0.0 || x > MAX_INPUT) {
        return NAN;
    }
    
    double sum = 0.0;
    double term = 1.0;
    int n = 1;
    
    while (n <= max_iter && fabs(term) > EPSILON) {
        term = pow(-1, n + 1) * pow(x, n) / n;
        if (isnan(term) || isinf(term)) {
            return NAN;
        }
        sum += term;
        n++;
    }
    
    return sum;
}

int main(void) {
    double input_value;
    char buffer[100];
    
    printf("Enter a positive number (0 < x <= %g): ", MAX_INPUT);
    
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
    
    if (input_value <= MIN_INPUT || input_value > MAX_INPUT) {
        fprintf(stderr, "Input out of valid range (%.10g, %g]\n", MIN_INPUT, MAX_INPUT);
        return EXIT_FAILURE;
    }
    
    double result = compute_series_sum(input_value, MAX_ITERATIONS);
    
    if (isnan(result)) {
        fprintf(stderr, "Computation failed - numerical error\n");
        return EXIT_FAILURE;
    }
    
    double reference = log(1.0 + input_value);
    
    printf("Series sum: %.15g\n", result);
    printf("Reference ln(1+x): %.15g\n", reference);
    printf("Absolute error: %.15g\n", fabs(result - reference));
    
    return EXIT_SUCCESS;
}