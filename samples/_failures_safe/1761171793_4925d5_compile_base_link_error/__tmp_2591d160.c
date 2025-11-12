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
    
    int dot_count = 0;
    int digit_count = 0;
    
    if (*str == '-' || *str == '+') {
        str++;
    }
    
    while (*str != '\0') {
        if (*str == '.') {
            dot_count++;
            if (dot_count > 1) {
                return 0;
            }
        } else if (*str >= '0' && *str <= '9') {
            digit_count++;
        } else {
            return 0;
        }
        str++;
    }
    
    return digit_count > 0;
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
    } else if (len == sizeof(input) - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    if (!is_valid_double(input)) {
        fprintf(stderr, "Invalid number format\n");
        return 1;
    }
    
    if (sscanf(input, "%lf", &x) != 1) {
        fprintf(stderr, "Error parsing number\n");
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
    } else if (len == sizeof(input) - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char* endptr;
    long temp_terms = strtol(input, &endptr, 10);
    if (*endptr != '\0' || temp_terms < 1 || temp_terms > 1000) {
        fprintf(stderr, "Invalid number of terms\n");
        return 1;
    }
    terms = (int)temp_terms;
    
    if (terms > 1000) {
        fprintf(stderr, "Too many terms\n");
        return 1;
    }
    
    double result = compute_series_sum(x, terms);
    double exp_result = exp(x);
    
    printf("Series approximation: %.10f\n", result);
    printf("Exponential function: %.10f\n", exp_result);
    printf("Absolute difference: %.10f\n", fabs(result - exp_result));
    
    return 0;
}