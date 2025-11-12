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
    
    if (val > INT_MAX || val < INT_MIN) {
        return 0;
    }
    
    *result = (int)val;
    return 1;
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN];
    double x;
    int terms;
    
    printf("Enter value for x: ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char* endptr;
    x = strtod(input_buffer, &endptr);
    if (endptr == input_buffer || *endptr != '\n') {
        fprintf(stderr, "Invalid number format\n");
        return 1;
    }
    
    if (!isfinite(x)) {
        fprintf(stderr, "Invalid x value\n");
        return 1;
    }
    
    printf("Enter number of terms: ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (!safe_str_to_int(input_buffer, &terms)) {
        fprintf(stderr, "Invalid terms format\n");
        return 1;
    }
    
    if (terms <= 0 || terms > 1000) {
        fprintf(stderr, "Terms must be between 1 and 1000\n");
        return 1;
    }
    
    double result = compute_series_sum(x, terms);
    double reference = exp(x);
    
    printf("Series sum: %.10f\n", result);
    printf("Reference (e^x): %.10f\n", reference);
    printf("Difference: %.10f\n", fabs(result - reference));
    
    return 0;
}