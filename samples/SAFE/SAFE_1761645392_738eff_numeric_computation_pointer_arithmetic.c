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
    
    printf("Enter polynomial coefficients from highest to lowest degree (space separated): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    token = strtok(input, " \t\n");
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
        token = strtok(NULL, " \t\n");
    }
    
    return count;
}

int main(void) {
    double coefficients[MAX_SIZE];
    double x_value, result;
    char input[256];
    int degree;
    
    degree = read_coefficients(coefficients, MAX_SIZE - 1);
    if (degree <= 0) {
        fprintf(stderr, "Error: Invalid coefficients input\n");
        return 1;
    }
    
    printf("Enter x value for polynomial evaluation: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error: Invalid x value input\n");
        return 1;
    }
    
    char *endptr;
    x_value = strtod(input, &endptr);
    if (endptr == input || *endptr != '\n' && *endptr != '\0') {
        fprintf(stderr, "Error: Invalid x value format\n");
        return 1;
    }
    
    if (!isfinite(x_value)) {
        fprintf(stderr, "Error: x value is not finite\n");
        return 1;
    }
    
    result = compute_polynomial(coefficients, degree - 1, x_value);
    
    if (!isfinite(result)) {
        fprintf(stderr, "Error: Result is not finite\n");
        return 1;
    }
    
    printf("Polynomial evaluation result: %.6f\n", result);
    
    return 0;
}