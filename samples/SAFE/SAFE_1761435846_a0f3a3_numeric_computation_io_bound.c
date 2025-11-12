//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_LEN 256

double compute_polynomial(double x, double coeffs[], int degree) {
    double result = 0.0;
    double term = 1.0;
    
    for (int i = 0; i <= degree; i++) {
        result += coeffs[i] * term;
        term *= x;
    }
    
    return result;
}

int parse_double(const char *str, double *value) {
    char *endptr;
    *value = strtod(str, &endptr);
    return (endptr != str && *endptr == '\0');
}

int parse_int(const char *str, int *value) {
    char *endptr;
    long temp = strtol(str, &endptr, 10);
    if (endptr != str && *endptr == '\0' && temp >= 0 && temp <= 100) {
        *value = (int)temp;
        return 1;
    }
    return 0;
}

int main() {
    char input[MAX_INPUT_LEN];
    int degree;
    double x;
    
    printf("Polynomial Calculator\n");
    printf("Enter polynomial degree (0-100): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (!parse_int(input, &degree)) {
        fprintf(stderr, "Invalid degree. Must be integer 0-100.\n");
        return 1;
    }
    
    double *coeffs = malloc((degree + 1) * sizeof(double));
    if (coeffs == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    printf("Enter %d coefficients (from constant term to highest degree):\n", degree + 1);
    
    for (int i = 0; i <= degree; i++) {
        printf("Coefficient %d: ", i);
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            fprintf(stderr, "Error reading input\n");
            free(coeffs);
            return 1;
        }
        
        input[strcspn(input, "\n")] = '\0';
        
        if (!parse_double(input, &coeffs[i])) {
            fprintf(stderr, "Invalid coefficient\n");
            free(coeffs);
            return 1;
        }
    }
    
    printf("Enter x value to evaluate: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        free(coeffs);
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (!parse_double(input, &x)) {
        fprintf(stderr, "Invalid x value\n");
        free(coeffs);
        return 1;
    }
    
    double result = compute_polynomial(x, coeffs, degree);
    
    printf("P(%g) = ", x);
    for (int i = 0; i <= degree; i++) {
        if (i == 0) {
            printf("%g", coeffs[i]);
        } else if (i == 1) {
            printf(" + %g*x", coeffs[i]);
        } else {
            printf(" + %g*x^%d", coeffs[i], i);
        }
    }
    printf("\n");
    
    printf("Result: %g\n", result);
    
    free(coeffs);
    return 0;
}