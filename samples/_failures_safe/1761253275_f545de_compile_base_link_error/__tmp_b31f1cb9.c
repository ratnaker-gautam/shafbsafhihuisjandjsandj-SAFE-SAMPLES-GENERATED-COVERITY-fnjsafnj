//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10
#define MAX_INPUT 1000000.0
#define MIN_INPUT -1000000.0

#define VALIDATE_INPUT(x) ((x) >= MIN_INPUT && (x) <= MAX_INPUT)
#define ABS(x) ((x) < 0 ? -(x) : (x))

double compute_series(double x, int max_iter) {
    double result = 0.0;
    double term = x;
    int n = 1;
    
    while (n <= max_iter && ABS(term) > EPSILON) {
        result += term;
        term = -term * x * x / ((2 * n) * (2 * n + 1));
        n++;
        
        if (n > MAX_ITERATIONS) {
            break;
        }
    }
    return result;
}

double compute_derivative(double x, double (*f)(double)) {
    double h = 1e-8;
    return (f(x + h) - f(x - h)) / (2 * h);
}

double sine_function(double x) {
    return compute_series(x, MAX_ITERATIONS);
}

int main(void) {
    double input_value;
    char buffer[256];
    
    printf("Enter a value between %.1f and %.1f: ", MIN_INPUT, MAX_INPUT);
    
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
    
    if (!VALIDATE_INPUT(input_value)) {
        fprintf(stderr, "Input out of valid range\n");
        return EXIT_FAILURE;
    }
    
    double result = compute_series(input_value, MAX_ITERATIONS);
    double derivative = compute_derivative(input_value, sine_function);
    
    if (!isfinite(result) || !isfinite(derivative)) {
        fprintf(stderr, "Numerical computation error\n");
        return EXIT_FAILURE;
    }
    
    printf("Series result: %.10f\n", result);
    printf("Numerical derivative: %.10f\n", derivative);
    printf("Library sin(x): %.10f\n", sin(input_value));
    
    return EXIT_SUCCESS;
}