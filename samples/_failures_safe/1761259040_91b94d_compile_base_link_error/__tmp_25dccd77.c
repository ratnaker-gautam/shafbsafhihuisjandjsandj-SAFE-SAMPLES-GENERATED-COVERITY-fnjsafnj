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
    
    for (int n = 1; n <= terms; n++) {
        term *= x / n;
        sum += term;
    }
    
    return sum + 1.0;
}

int safe_str_to_int(const char* str, int* result) {
    char* endptr;
    long val = strtol(str, &endptr, 10);
    
    if (endptr == str || *endptr != '\0') {
        return 0;
    }
    
    if (val < INT_MIN || val > INT_MAX) {
        return 0;
    }
    
    *result = (int)val;
    return 1;
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 1];
    double x;
    int terms;
    
    printf("Series approximation calculator\n");
    printf("Computing exp(x) using Taylor series\n\n");
    
    while (1) {
        printf("Enter x value (or 'quit' to exit): ");
        
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input_buffer);
        if (len > 0 && input_buffer[len - 1] == '\n') {
            input_buffer[len - 1] = '\0';
            len--;
        }
        
        if (len == 0) {
            continue;
        }
        
        if (strcmp(input_buffer, "quit") == 0) {
            break;
        }
        
        char* endptr;
        x = strtod(input_buffer, &endptr);
        if (endptr == input_buffer || *endptr != '\0') {
            printf("Invalid number format. Please try again.\n");
            continue;
        }
        
        if (!isfinite(x)) {
            printf("Value must be finite. Please try again.\n");
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
        
        if (!safe_str_to_int(input_buffer, &terms)) {
            printf("Invalid integer format. Please try again.\n");
            continue;
        }
        
        if (terms < 1 || terms > 1000) {
            printf("Terms must be between 1 and 1000. Please try again.\n");
            continue;
        }
        
        double result = compute_series_sum(x, terms);
        double reference = exp(x);
        double error = fabs(result - reference);
        
        printf("\nResults for x = %.6f, terms = %d:\n", x, terms);
        printf("Series approximation: %.12f\n", result);
        printf("Reference exp(x):     %.12f\n", reference);
        printf("Absolute error:       %.12f\n\n", error);
    }
    
    printf("Goodbye!\n");
    return 0;
}