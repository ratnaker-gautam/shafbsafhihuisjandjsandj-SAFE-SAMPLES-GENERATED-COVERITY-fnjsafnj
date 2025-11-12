//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10
#define MAX_INPUT 1000000.0
#define MIN_INPUT 1e-10

double compute_series(double x, int *iterations) {
    if (x <= 0.0 || x > MAX_INPUT) {
        return -1.0;
    }
    
    double sum = 0.0;
    double term = x;
    int n = 1;
    
    while (n <= MAX_ITERATIONS && fabs(term) > EPSILON) {
        sum += term;
        n++;
        term = pow(-1, n + 1) * pow(x, n) / n;
        
        if (isnan(sum) || isinf(sum)) {
            return -1.0;
        }
    }
    
    *iterations = n - 1;
    return sum;
}

int main(void) {
    double input;
    char buffer[256];
    
    printf("Enter a positive number (1e-10 to 1e6): ");
    
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
    
    if (input <= MIN_INPUT || input > MAX_INPUT) {
        fprintf(stderr, "Input out of valid range\n");
        return EXIT_FAILURE;
    }
    
    int iterations;
    double result = compute_series(input, &iterations);
    
    if (result == -1.0) {
        fprintf(stderr, "Computation failed\n");
        return EXIT_FAILURE;
    }
    
    double reference = log(1.0 + input);
    double error = fabs(result - reference);
    
    printf("Input: %.10g\n", input);
    printf("Series result: %.10g\n", result);
    printf("Reference ln(1+x): %.10g\n", reference);
    printf("Iterations: %d\n", iterations);
    printf("Absolute error: %.2e\n", error);
    
    return EXIT_SUCCESS;
}