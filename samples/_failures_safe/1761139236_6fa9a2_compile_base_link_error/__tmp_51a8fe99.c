//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
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

double validate_input(const char* str) {
    char* endptr;
    double val = strtod(str, &endptr);
    
    if (endptr == str || *endptr != '\0') {
        return -1.0;
    }
    
    if (val < -MAX_INPUT || val > MAX_INPUT) {
        return -1.0;
    }
    
    if (fabs(val) < MIN_INPUT && val != 0.0) {
        return -1.0;
    }
    
    return val;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number>\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    double input = validate_input(argv[1]);
    if (input == -1.0) {
        fprintf(stderr, "Error: Invalid input. Must be a number between %.0f and %.0f\n", 
                -MAX_INPUT, MAX_INPUT);
        return EXIT_FAILURE;
    }
    
    double result = compute_series(input, MAX_ITERATIONS);
    double reference = exp(input) - 1.0;
    
    printf("Input: %.6f\n", input);
    printf("Series result: %.10f\n", result);
    printf("Reference (e^x - 1): %.10f\n", reference);
    printf("Absolute error: %.10f\n", fabs(result - reference));
    
    return EXIT_SUCCESS;
}