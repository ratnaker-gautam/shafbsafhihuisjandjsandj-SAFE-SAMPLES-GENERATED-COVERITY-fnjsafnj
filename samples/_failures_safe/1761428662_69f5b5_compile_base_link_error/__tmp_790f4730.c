//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_LEN 256

double calculate_polynomial(double x, double coeffs[], int degree) {
    double result = 0.0;
    for (int i = 0; i <= degree; i++) {
        result += coeffs[i] * pow(x, i);
    }
    return result;
}

int read_double(const char *prompt, double *value) {
    char buffer[MAX_INPUT_LEN];
    char *endptr;
    
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    
    buffer[strcspn(buffer, "\n")] = 0;
    *value = strtod(buffer, &endptr);
    
    if (endptr == buffer || *endptr != '\0') {
        return 0;
    }
    
    return 1;
}

int read_int(const char *prompt, int *value) {
    char buffer[MAX_INPUT_LEN];
    char *endptr;
    long temp;
    
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    
    buffer[strcspn(buffer, "\n")] = 0;
    temp = strtol(buffer, &endptr, 10);
    
    if (endptr == buffer || *endptr != '\0') {
        return 0;
    }
    
    if (temp < INT_MIN || temp > INT_MAX) {
        return 0;
    }
    
    *value = (int)temp;
    return 1;
}

int main() {
    int degree;
    double x_value;
    double coefficients[20];
    double result;
    
    printf("Polynomial Calculator\n");
    printf("====================\n\n");
    
    if (!read_int("Enter polynomial degree (0-19): ", &degree)) {
        printf("Error: Invalid degree input\n");
        return 1;
    }
    
    if (degree < 0 || degree > 19) {
        printf("Error: Degree must be between 0 and 19\n");
        return 1;
    }
    
    printf("Enter coefficients from constant term to x^%d:\n", degree);
    for (int i = 0; i <= degree; i++) {
        char prompt[50];
        snprintf(prompt, sizeof(prompt), "Coefficient for x^%d: ", i);
        
        if (!read_double(prompt, &coefficients[i])) {
            printf("Error: Invalid coefficient input\n");
            return 1;
        }
    }
    
    if (!read_double("Enter x value to evaluate: ", &x_value)) {
        printf("Error: Invalid x value input\n");
        return 1;
    }
    
    result = calculate_polynomial(x_value, coefficients, degree);
    
    printf("\nPolynomial: ");
    for (int i = 0; i <= degree; i++) {
        if (i == 0) {
            printf("%.2f", coefficients[i]);
        } else {
            printf(" + %.2f*x^%d", coefficients[i], i);
        }
    }
    
    printf("\nResult at x = %.2f: %.6f\n", x_value, result);
    
    return 0;
}