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

int main() {
    char input_buffer[MAX_INPUT_LEN];
    double x;
    int terms;
    
    printf("Series approximation calculator\n");
    printf("Computing sum of x^i/i! from i=1 to n\n");
    
    while (1) {
        printf("\nEnter x value (or 'quit' to exit): ");
        
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            break;
        }
        
        input_buffer[strcspn(input_buffer, "\n")] = '\0';
        
        if (strcmp(input_buffer, "quit") == 0) {
            break;
        }
        
        char* endptr;
        x = strtod(input_buffer, &endptr);
        if (endptr == input_buffer || *endptr != '\0') {
            printf("Invalid number format\n");
            continue;
        }
        
        if (!isfinite(x)) {
            printf("Invalid floating point value\n");
            continue;
        }
        
        printf("Enter number of terms (1-1000): ");
        
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            break;
        }
        
        input_buffer[strcspn(input_buffer, "\n")] = '\0';
        
        if (!safe_str_to_int(input_buffer, &terms)) {
            printf("Invalid integer format\n");
            continue;
        }
        
        if (terms < 1 || terms > 1000) {
            printf("Terms must be between 1 and 1000\n");
            continue;
        }
        
        double result = compute_series_sum(x, terms);
        double exact = exp(x) - 1.0;
        double error = fabs(result - exact);
        
        printf("Approximation: %.10g\n", result);
        printf("Exact value (e^x - 1): %.10g\n", exact);
        printf("Absolute error: %.10g\n", error);
        printf("Relative error: %.10g\n", error / fabs(exact));
    }
    
    return 0;
}