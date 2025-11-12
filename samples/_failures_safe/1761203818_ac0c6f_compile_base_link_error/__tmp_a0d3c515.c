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
    double sum = 0.0;
    double term = x;
    int n = 1;
    
    while (n <= max_iter && fabs(term) > EPSILON) {
        sum += term;
        n++;
        term = term * x / n;
    }
    
    return sum;
}

double validate_input(double input) {
    if (input < 0) {
        return -input;
    }
    if (input > MAX_INPUT) {
        return MAX_INPUT;
    }
    if (input < MIN_INPUT && input > 0) {
        return MIN_INPUT;
    }
    return input;
}

int main(void) {
    double input_value;
    char buffer[100];
    char *endptr;
    
    printf("Enter a positive number for series computation: ");
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    input_value = strtod(buffer, &endptr);
    if (endptr == buffer) {
        fprintf(stderr, "Invalid input: not a number\n");
        return EXIT_FAILURE;
    }
    
    if (input_value <= 0) {
        fprintf(stderr, "Input must be positive\n");
        return EXIT_FAILURE;
    }
    
    input_value = validate_input(input_value);
    
    double result = compute_series(input_value, MAX_ITERATIONS);
    double reference = exp(input_value) - 1.0;
    
    printf("Input value: %.6f\n", input_value);
    printf("Series result: %.10f\n", result);
    printf("Reference (e^x - 1): %.10f\n", reference);
    printf("Absolute difference: %.10f\n", fabs(result - reference));
    
    return EXIT_SUCCESS;
}