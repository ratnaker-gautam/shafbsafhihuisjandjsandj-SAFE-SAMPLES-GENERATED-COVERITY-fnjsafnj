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

int safe_str_to_int(const char* str, int* result) {
    char* endptr;
    long val = strtol(str, &endptr, 10);
    
    if (endptr == str || *endptr != '\0') {
        return 0;
    }
    
    if (val < INT_MIN || val > INT_MAX) {
        return 0;
    }
    
    *result = (int)val;
    return 1;
}

int safe_str_to_double(const char* str, double* result) {
    char* endptr;
    double val = strtod(str, &endptr);
    
    if (endptr == str || *endptr != '\0') {
        return 0;
    }
    
    *result = val;
    return 1;
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN];
    double x;
    int terms;
    
    printf("Series approximation calculator (e^x approximation)\n");
    printf("Enter value for x: ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (!safe_str_to_double(input_buffer, &x)) {
        fprintf(stderr, "Invalid number format for x\n");
        return 1;
    }
    
    printf("Enter number of terms (1-1000): ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (!safe_str_to_int(input_buffer, &terms)) {
        fprintf(stderr, "Invalid number format for terms\n");
        return 1;
    }
    
    if (terms < 1 || terms > 1000) {
        fprintf(stderr, "Terms must be between 1 and 1000\n");
        return 1;
    }
    
    double result = compute_series_sum(x, terms);
    double reference = exp(x);
    double error = fabs(result - reference);
    
    printf("Approximation: %.10f\n", result);
    printf("Reference (exp(x)): %.10f\n", reference);
    printf("Absolute error: %.10f\n", error);
    printf("Relative error: %.10f%%\n", (error / fabs(reference)) * 100.0);
    
    return 0;
}