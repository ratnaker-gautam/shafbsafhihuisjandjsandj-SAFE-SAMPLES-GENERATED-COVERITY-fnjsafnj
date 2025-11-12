//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_LEN 256

double compute_series_sum(int terms) {
    double sum = 0.0;
    for (int i = 1; i <= terms; i++) {
        if (i == 0) {
            return -1.0;
        }
        double denominator = (double)i * (double)i;
        if (isinf(denominator) || isnan(denominator)) {
            return -1.0;
        }
        double term = 1.0 / denominator;
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

int get_valid_integer(const char* prompt, int min, int max) {
    char buffer[MAX_INPUT_LEN];
    long value;
    char* endptr;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
            len--;
        }
        
        if (len == 0) {
            continue;
        }
        
        int valid = 1;
        for (size_t i = 0; i < len; i++) {
            if (i == 0 && buffer[i] == '-') continue;
            if (buffer[i] < '0' || buffer[i] > '9') {
                valid = 0;
                break;
            }
        }
        
        if (!valid) {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        value = strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\0') {
            printf("Invalid input format.\n");
            continue;
        }
        
        if (value < min || value > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        return (int)value;
    }
}

int main(void) {
    printf("Series Sum Calculator\n");
    printf("Computes sum of 1/n^2 from n=1 to specified terms\n\n");
    
    int terms = get_valid_integer("Enter number of terms (1-1000000): ", 1, 1000000);
    if (terms == -1) {
        return 1;
    }
    
    printf("Computing sum for %d terms...\n", terms);
    
    double result = compute_series_sum(terms);
    if (result == -1.0) {
        printf("Error: Numerical overflow occurred during computation.\n");
        return 1;
    }
    
    double pi_squared_over_six = 3.14159265358979323846 * 3.14159265358979323846 / 6.0;
    printf("Result: %.15f\n", result);
    printf("Reference (π²/6): %.15f\n", pi_squared_over_six);
    printf("Difference: %.15f\n", fabs(result - pi_squared_over_six));
    
    return 0;
}