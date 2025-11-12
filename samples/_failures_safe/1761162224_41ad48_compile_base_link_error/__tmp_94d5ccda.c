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
    char input_buffer[MAX_INPUT_LEN + 1];
    double x;
    int terms;
    
    printf("Enter value for x: ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    } else if (len == MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    if (!is_valid_double(input_buffer)) {
        fprintf(stderr, "Invalid number format\n");
        return 1;
    }
    
    x = atof(input_buffer);
    
    printf("Enter number of terms (1-1000): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    } else if (len == MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char* endptr;
    long temp = strtol(input_buffer, &endptr, 10);
    if (*endptr != '\0' || temp <= 0 || temp > 1000) {
        fprintf(stderr, "Invalid number of terms\n");
        return 1;
    }
    terms = (int)temp;
    
    if (terms < 1 || terms > 1000) {
        fprintf(stderr, "Terms must be between 1 and 1000\n");
        return 1;
    }
    
    double result = compute_series_sum(x, terms);
    double reference = exp(x);
    
    printf("Series approximation: %.10f\n", result);
    printf("Reference exp(x):    %.10f\n", reference);
    printf("Absolute error:      %.10f\n", fabs(result - reference));
    
    return 0;
}