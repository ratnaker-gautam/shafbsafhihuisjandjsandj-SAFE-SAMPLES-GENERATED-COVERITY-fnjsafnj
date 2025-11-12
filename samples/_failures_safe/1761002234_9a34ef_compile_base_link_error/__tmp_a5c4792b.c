//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: numeric_computation
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

int validate_double_input(const char* input, double* result) {
    char* endptr;
    double value = strtod(input, &endptr);
    
    if (endptr == input || *endptr != '\0') {
        return 0;
    }
    
    if (value == HUGE_VAL || value == -HUGE_VAL || value != value) {
        return 0;
    }
    
    *result = value;
    return 1;
}

int validate_int_input(const char* input, int* result) {
    char* endptr;
    long value = strtol(input, &endptr, 10);
    
    if (endptr == input || *endptr != '\0') {
        return 0;
    }
    
    if (value < 1 || value > 1000) {
        return 0;
    }
    
    *result = (int)value;
    return 1;
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 1];
    double x;
    int terms;
    
    printf("Series Approximation Calculator\n");
    printf("Computing e^x approximation using Taylor series\n\n");
    
    while (1) {
        printf("Enter x value (or 'quit' to exit): ");
        
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input_buffer);
        if (len > 0 && input_buffer[len - 1] == '\n') {
            input_buffer[len - 1] = '\0';
        }
        
        if (strcmp(input_buffer, "quit") == 0) {
            break;
        }
        
        if (!validate_double_input(input_buffer, &x)) {
            printf("Invalid input. Please enter a valid number.\n");
            continue;
        }
        
        printf("Enter number of terms (1-1000): ");
        
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            break;
        }
        
        len = strlen(input_buffer);
        if (len > 0 && input_buffer[len - 1] == '\n') {
            input_buffer[len - 1] = '\0';
        }
        
        if (!validate_int_input(input_buffer, &terms)) {
            printf("Invalid number of terms. Please enter an integer between 1 and 1000.\n");
            continue;
        }
        
        double result = compute_series_sum(x, terms);
        double reference = exp(x);
        double error = fabs(result - reference);
        
        printf("Approximation: %.10f\n", result);
        printf("Reference (exp(x)): %.10f\n", reference);
        printf("Absolute error: %.10f\n", error);
        printf("Relative error: %.10f%%\n\n", (error / fabs(reference)) * 100.0);
    }
    
    printf("Goodbye!\n");
    return 0;
}