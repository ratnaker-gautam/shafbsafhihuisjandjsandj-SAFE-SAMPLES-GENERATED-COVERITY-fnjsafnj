//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_LEN 256

double compute_series_sum(double x, int terms) {
    if (terms <= 0) return 0.0;
    
    double sum = 0.0;
    double term = 1.0;
    
    for (int i = 1; i <= terms; i++) {
        term *= x / i;
        sum += term;
    }
    
    return sum;
}

int safe_str_to_int(const char* str, int* result) {
    if (str == NULL || result == NULL) return 0;
    
    char* endptr;
    long val = strtol(str, &endptr, 10);
    
    if (endptr == str || *endptr != '\0') return 0;
    if (val < INT_MIN || val > INT_MAX) return 0;
    
    *result = (int)val;
    return 1;
}

int safe_str_to_double(const char* str, double* result) {
    if (str == NULL || result == NULL) return 0;
    
    char* endptr;
    double val = strtod(str, &endptr);
    
    if (endptr == str || *endptr != '\0') return 0;
    if (val == HUGE_VAL || val == -HUGE_VAL) return 0;
    
    *result = val;
    return 1;
}

int main() {
    char input_buffer[MAX_INPUT_LEN + 1];
    double x;
    int terms;
    
    printf("Series Sum Calculator (e^x approximation)\n");
    printf("Enter x value: ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (!safe_str_to_double(input_buffer, &x)) {
        fprintf(stderr, "Invalid x value\n");
        return 1;
    }
    
    printf("Enter number of terms: ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (!safe_str_to_int(input_buffer, &terms)) {
        fprintf(stderr, "Invalid number of terms\n");
        return 1;
    }
    
    if (terms <= 0) {
        fprintf(stderr, "Number of terms must be positive\n");
        return 1;
    }
    
    if (terms > 1000) {
        fprintf(stderr, "Too many terms (max 1000)\n");
        return 1;
    }
    
    double result = compute_series_sum(x, terms);
    double reference = exp(x);
    double error = fabs(result - reference);
    
    printf("Approximation: %.10f\n", result);
    printf("Actual e^x:    %.10f\n", reference);
    printf("Absolute error: %.10f\n", error);
    printf("Relative error: %.10f%%\n", (error / fabs(reference)) * 100.0);
    
    return 0;
}