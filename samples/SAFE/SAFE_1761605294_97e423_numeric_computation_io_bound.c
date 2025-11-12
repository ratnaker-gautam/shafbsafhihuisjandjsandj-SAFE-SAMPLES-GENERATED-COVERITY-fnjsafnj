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
            return sum;
        }
        double denominator = i * i;
        if (denominator == 0.0) {
            return sum;
        }
        double term = 1.0 / denominator;
        if (isinf(term) || isnan(term)) {
            return sum;
        }
        sum += term;
        if (isinf(sum) || isnan(sum)) {
            return sum;
        }
    }
    return sum;
}

int get_valid_integer(const char* prompt, int min_val, int max_val) {
    char buffer[MAX_INPUT_LEN];
    char* endptr;
    long value;
    
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
        
        value = strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\0') {
            continue;
        }
        
        if (value < min_val || value > max_val) {
            continue;
        }
        
        if (value > INT_MAX || value < INT_MIN) {
            continue;
        }
        
        return (int)value;
    }
}

int main(void) {
    printf("Series Sum Calculator\n");
    printf("Computes sum of 1/n^2 from n=1 to N\n\n");
    
    int terms = get_valid_integer("Enter number of terms (1-1000000): ", 1, 1000000);
    if (terms == -1) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    printf("Computing sum for %d terms...\n", terms);
    
    double result = compute_series_sum(terms);
    double pi_squared = 3.14159265358979323846 * 3.14159265358979323846;
    double expected = pi_squared / 6.0;
    double error = fabs(result - expected);
    
    printf("Computed sum: %.15f\n", result);
    printf("Theoretical limit (π²/6): %.15f\n", expected);
    printf("Absolute error: %.15f\n", error);
    
    return 0;
}