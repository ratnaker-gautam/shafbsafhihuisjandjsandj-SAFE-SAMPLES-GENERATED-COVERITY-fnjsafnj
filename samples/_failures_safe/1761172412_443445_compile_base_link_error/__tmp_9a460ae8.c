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
    
    return sum + 1.0;
}

int is_valid_double(const char* str) {
    if (str == NULL || *str == '\0') {
        return 0;
    }
    
    int decimal_points = 0;
    int digits = 0;
    
    if (*str == '-' || *str == '+') {
        str++;
    }
    
    while (*str) {
        if (*str == '.') {
            decimal_points++;
            if (decimal_points > 1) {
                return 0;
            }
        } else if (*str >= '0' && *str <= '9') {
            digits++;
        } else {
            return 0;
        }
        str++;
    }
    
    return digits > 0;
}

int main(void) {
    char input[MAX_INPUT_LEN];
    double x;
    int terms;
    
    printf("Enter value for x: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_double(input)) {
        fprintf(stderr, "Invalid number format\n");
        return 1;
    }
    
    char* endptr;
    x = strtod(input, &endptr);
    if (endptr == input || *endptr != '\0') {
        fprintf(stderr, "Invalid number conversion\n");
        return 1;
    }
    
    printf("Enter number of terms (1-1000): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    terms = atoi(input);
    if (terms < 1 || terms > 1000) {
        fprintf(stderr, "Number of terms must be between 1 and 1000\n");
        return 1;
    }
    
    double result = compute_series_sum(x, terms);
    double exp_result = exp(x);
    
    printf("Series approximation: %.15f\n", result);
    printf("Exponential function: %.15f\n", exp_result);
    printf("Absolute difference: %.15f\n", fabs(result - exp_result));
    
    return 0;
}