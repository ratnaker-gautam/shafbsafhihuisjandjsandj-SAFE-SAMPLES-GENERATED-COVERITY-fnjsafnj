//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_SIZE 100

double compute_polynomial(double *coeffs, int degree, double x) {
    double result = 0.0;
    double *ptr = coeffs + degree;
    
    while (ptr >= coeffs) {
        result = result * x + *ptr;
        ptr--;
    }
    
    return result;
}

int read_coefficients(double *buffer, int max_degree) {
    char input[1024];
    char *token;
    int count = 0;
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    token = strtok(input, " ");
    while (token != NULL && count <= max_degree) {
        char *endptr;
        double value = strtod(token, &endptr);
        
        if (endptr == token || *endptr != '\0') {
            return -1;
        }
        
        if (!isfinite(value)) {
            return -1;
        }
        
        *(buffer + count) = value;
        count++;
        token = strtok(NULL, " ");
    }
    
    return count - 1;
}

int main(void) {
    double coefficients[MAX_SIZE];
    double x_value;
    int degree;
    char input[256];
    
    printf("Enter polynomial coefficients (space-separated, highest degree first): ");
    degree = read_coefficients(coefficients, MAX_SIZE - 1);
    
    if (degree < 0 || degree >= MAX_SIZE) {
        fprintf(stderr, "Error: Invalid coefficients\n");
        return 1;
    }
    
    printf("Enter x value: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    char *endptr;
    x_value = strtod(input, &endptr);
    if (endptr == input || *endptr != '\0' || !isfinite(x_value)) {
        fprintf(stderr, "Error: Invalid x value\n");
        return 1;
    }
    
    double result = compute_polynomial(coefficients, degree, x_value);
    
    if (!isfinite(result)) {
        fprintf(stderr, "Error: Computation resulted in non-finite value\n");
        return 1;
    }
    
    printf("P(%.6f) = %.6f\n", x_value, result);
    
    return 0;
}