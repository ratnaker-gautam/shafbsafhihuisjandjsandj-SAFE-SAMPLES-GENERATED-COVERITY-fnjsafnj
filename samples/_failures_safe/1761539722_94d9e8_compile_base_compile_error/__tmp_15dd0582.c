//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>
#include <float.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10
#define MAX_INPUT 1000000.0
#define MIN_INPUT 1e-10

double compute_series(double x, int max_iter) {
    if (x <= 0.0 || x > MAX_INPUT) {
        return -1.0;
    }
    double sum = 0.0;
    double term = x;
    for (int n = 1; n <= max_iter; n++) {
        if (n % 2 == 0) {
            sum -= term / n;
        } else {
            sum += term / n;
        }
        term *= x;
        if (fabs(term) < EPSILON) {
            break;
        }
        if (term > DBL_MAX / x) {
            break;
        }
    }
    return sum;
}

int main() {
    double input_value;
    char buffer[256];
    
    printf("Enter a positive number (1e-10 to 1e6): ");
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
        fprintf(stderr, "Input out of valid range\n");
        return EXIT_FAILURE;
    }
    
    if (!isfinite(input_value)) {
        fprintf(stderr, "Input is not finite\n");
        return EXIT_FAILURE;
    }
    
    double result = compute_series(input_value, MAX_ITERATIONS);
    if (result < 0.0) {
        fprintf(stderr, "Computation failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Series result for x=%.6f: %.10f\n", input_value, result);
    printf("Reference ln(1+x): %.10f\n", log1p(input_value));
    
    return EXIT_SUCCESS;
}