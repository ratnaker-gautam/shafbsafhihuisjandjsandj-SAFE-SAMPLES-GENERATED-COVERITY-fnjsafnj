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
    
    for (int i = 1; i <= terms; i++) {
        term *= x / i;
        sum += term;
    }
    
    return sum;
}

int validate_double_input(const char* input, double* result) {
    char* endptr;
    *result = strtod(input, &endptr);
    
    if (endptr == input || *endptr != '\n') {
        return 0;
    }
    
    if (*result == HUGE_VAL || *result == -HUGE_VAL) {
        return 0;
    }
    
    return 1;
}

int validate_int_input(const char* input, int* result) {
    char* endptr;
    long val = strtol(input, &endptr, 10);
    
    if (endptr == input || *endptr != '\n') {
        return 0;
    }
    
    if (val < 1 || val > 1000 || val > INT_MAX) {
        return 0;
    }
    
    *result = (int)val;
    return 1;
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 2];
    double x;
    int terms;
    
    printf("Series Sum Calculator (e^x approximation)\n");
    printf("Enter x value: ");
    
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
    
    if (!validate_int_input(input_buffer, &terms)) {
        fprintf(stderr, "Invalid number of terms\n");
        return 1;
    }
    
    double result = compute_series_sum(x, terms);
    double reference = exp(x);
    double error = fabs(result - reference);
    
    printf("Series sum: %.10f\n", result);
    printf("Reference (e^x): %.10f\n", reference);
    printf("Absolute error: %.10f\n", error);
    printf("Relative error: %.10f%%\n", (error / fabs(reference)) * 100.0);
    
    return 0;
}