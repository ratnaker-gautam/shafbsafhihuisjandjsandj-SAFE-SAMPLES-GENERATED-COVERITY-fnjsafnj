//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_LEN 256

double compute_series_sum(double x, int terms) {
    double sum = 0.0;
    double term = 1.0;
    
    for (int n = 0; n < terms; n++) {
        if (n > 0) {
            term *= x / n;
        }
        sum += term;
    }
    
    return sum;
}

int validate_integer_input(const char* input, int* value) {
    char* endptr;
    long val = strtol(input, &endptr, 10);
    
    if (endptr == input || *endptr != '\n') {
        return 0;
    }
    
    if (val < 1 || val > 1000) {
        return 0;
    }
    
    *value = (int)val;
    return 1;
}

int validate_double_input(const char* input, double* value) {
    char* endptr;
    double val = strtod(input, &endptr);
    
    if (endptr == input || *endptr != '\n') {
        return 0;
    }
    
    if (val < -100.0 || val > 100.0) {
        return 0;
    }
    
    *value = val;
    return 1;
}

int main() {
    char input_buffer[MAX_INPUT_LEN + 2];
    double x;
    int terms;
    
    printf("Series Sum Calculator (approximates e^x)\n");
    printf("Enter x value (-100 to 100): ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (strlen(input_buffer) >= MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    if (!validate_double_input(input_buffer, &x)) {
        fprintf(stderr, "Invalid x value\n");
        return 1;
    }
    
    printf("Enter number of terms (1-1000): ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (strlen(input_buffer) >= MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    if (!validate_integer_input(input_buffer, &terms)) {
        fprintf(stderr, "Invalid number of terms\n");
        return 1;
    }
    
    double result = compute_series_sum(x, terms);
    double reference = exp(x);
    double error = fabs(result - reference);
    
    printf("Computed sum: %.15f\n", result);
    printf("Reference (exp(x)): %.15f\n", reference);
    printf("Absolute error: %.15f\n", error);
    
    return 0;
}