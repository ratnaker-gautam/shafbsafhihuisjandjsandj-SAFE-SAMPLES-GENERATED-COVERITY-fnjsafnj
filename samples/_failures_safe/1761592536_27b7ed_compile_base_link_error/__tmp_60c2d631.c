//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define MAX_ITERATIONS 1000
#define TOLERANCE 1e-10
#define MAX_INPUT 1000000.0
#define MIN_INPUT 1e-10

double compute_series(double x, int *iterations) {
    if (x <= 0.0 || x > MAX_INPUT) {
        return -1.0;
    }
    
    double sum = 0.0;
    double term = 1.0;
    int n = 1;
    
    while (fabs(term) > TOLERANCE && n <= MAX_ITERATIONS) {
        term = pow(-1.0, n + 1) * pow(x, n) / n;
        if (isnan(term) || isinf(term)) {
            return -1.0;
        }
        sum += term;
        n++;
    }
    
    *iterations = n - 1;
    return sum;
}

int main(void) {
    double input_value;
    char buffer[256];
    
    printf("Enter a positive number (1e-10 to 1000000): ");
    
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
    
    int iterations;
    double result = compute_series(input_value, &iterations);
    
    if (result == -1.0) {
        fprintf(stderr, "Computation failed\n");
        return EXIT_FAILURE;
    }
    
    double reference = log(1.0 + input_value);
    
    printf("Input value: %.10g\n", input_value);
    printf("Series result: %.10g\n", result);
    printf("Reference ln(1+x): %.10g\n", reference);
    printf("Absolute error: %.10g\n", fabs(result - reference));
    printf("Iterations used: %d\n", iterations);
    
    return EXIT_SUCCESS;
}