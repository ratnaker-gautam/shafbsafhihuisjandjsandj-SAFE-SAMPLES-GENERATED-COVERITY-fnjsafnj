//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_LEN 256

double calculate_series_sum(int terms) {
    double sum = 0.0;
    for (int i = 1; i <= terms; i++) {
        double term = 1.0 / (i * i);
        if (isinf(term) || isnan(term)) {
            return -1.0;
        }
        sum += term;
        if (isinf(sum) || isnan(sum)) {
            return -1.0;
        }
    }
    return sum;
}

int validate_integer_input(const char* input, int* result) {
    if (input == NULL || result == NULL) {
        return 0;
    }
    
    char* endptr;
    long val = strtol(input, &endptr, 10);
    
    if (endptr == input || *endptr != '\0') {
        return 0;
    }
    
    if (val < 1 || val > 1000000) {
        return 0;
    }
    
    *result = (int)val;
    return 1;
}

int main(void) {
    char input[MAX_INPUT_LEN];
    int terms;
    
    printf("Series Sum Calculator\n");
    printf("Computes sum of 1/n^2 from n=1 to N terms\n");
    printf("Enter number of terms (1-1000000): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    } else if (len == sizeof(input) - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    if (!validate_integer_input(input, &terms)) {
        fprintf(stderr, "Invalid input. Please enter an integer between 1 and 1000000\n");
        return 1;
    }
    
    double result = calculate_series_sum(terms);
    if (result < 0) {
        fprintf(stderr, "Numerical error in computation\n");
        return 1;
    }
    
    double pi_squared_over_six = 3.14159265358979323846 * 3.14159265358979323846 / 6.0;
    printf("Sum of first %d terms: %.15f\n", terms, result);
    printf("Approximation to pi^2/6: %.15f\n", pi_squared_over_six);
    printf("Difference: %.15f\n", fabs(result - pi_squared_over_six));
    
    return 0;
}