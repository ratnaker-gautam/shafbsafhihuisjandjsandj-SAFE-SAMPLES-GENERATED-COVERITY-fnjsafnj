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
        if (i == 0) {
            return -1.0;
        }
        double denominator = (double)i * (double)i;
        if (denominator == 0.0) {
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

int get_valid_integer(const char* prompt, int min_val, int max_val) {
    char input[MAX_INPUT_LEN];
    char* endptr;
    long value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(input, sizeof(input), stdin) == NULL) {
            return -1;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
            len--;
        }
        
        if (len == 0) {
            continue;
        }
        
        int valid = 1;
        for (size_t i = 0; i < len; i++) {
            if (i == 0 && input[i] == '-') continue;
            if (input[i] < '0' || input[i] > '9') {
                valid = 0;
                break;
            }
        }
        
        if (!valid) {
            printf("Invalid input. Please enter a valid integer.\n");
            continue;
        }
        
        value = strtol(input, &endptr, 10);
        if (endptr == input || *endptr != '\0') {
            printf("Invalid input format.\n");
            continue;
        }
        
        if (value < min_val || value > max_val) {
            printf("Value must be between %d and %d.\n", min_val, max_val);
            continue;
        }
        
        if (value > INT_MAX || value < INT_MIN) {
            printf("Value out of integer range.\n");
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
    
    printf("\nComputing sum for %d terms...\n", terms);
    
    double result = calculate_series_sum(terms);
    if (result == -1.0) {
        printf("Error: Numerical overflow occurred during computation.\n");
        return 1;
    }
    
    double pi_squared_over_six = (3.14159265358979323846 * 3.14159265358979323846) / 6.0;
    printf("Series sum: %.15f\n", result);
    printf("Reference value (π²/6): %.15f\n", pi_squared_over_six);
    printf("Difference: %.15f\n", fabs(result - pi_squared_over_six));
    
    return 0;
}