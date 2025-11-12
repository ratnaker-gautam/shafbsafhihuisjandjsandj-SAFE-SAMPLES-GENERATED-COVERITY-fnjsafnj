//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_LEN 256

double compute_polynomial(double x, double coeffs[], int degree) {
    double result = 0.0;
    double x_power = 1.0;
    
    for (int i = 0; i <= degree; i++) {
        result += coeffs[i] * x_power;
        x_power *= x;
    }
    
    return result;
}

int read_double(const char* prompt, double* value) {
    char buffer[MAX_INPUT_LEN];
    
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    
    char* endptr;
    *value = strtod(buffer, &endptr);
    
    if (endptr == buffer || *endptr != '\n') {
        return 0;
    }
    
    return 1;
}

int read_int(const char* prompt, int* value) {
    char buffer[MAX_INPUT_LEN];
    
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    
    char* endptr;
    long temp = strtol(buffer, &endptr, 10);
    
    if (endptr == buffer || *endptr != '\n') {
        return 0;
    }
    
    if (temp < 0 || temp > 100) {
        return 0;
    }
    
    *value = (int)temp;
    return 1;
}

int main(void) {
    double x;
    int degree;
    
    printf("Polynomial Evaluator\n");
    printf("===================\n\n");
    
    if (!read_double("Enter value for x: ", &x)) {
        fprintf(stderr, "Error: Invalid input for x\n");
        return 1;
    }
    
    if (!read_int("Enter polynomial degree (0-100): ", &degree)) {
        fprintf(stderr, "Error: Invalid degree\n");
        return 1;
    }
    
    double coefficients[101];
    printf("\nEnter coefficients from constant term to highest degree:\n");
    
    for (int i = 0; i <= degree; i++) {
        char prompt[50];
        snprintf(prompt, sizeof(prompt), "Coefficient for x^%d: ", i);
        
        if (!read_double(prompt, &coefficients[i])) {
            fprintf(stderr, "Error: Invalid coefficient\n");
            return 1;
        }
    }
    
    double result = compute_polynomial(x, coefficients, degree);
    
    printf("\nPolynomial: ");
    for (int i = degree; i >= 0; i--) {
        if (i == degree) {
            printf("%.2f*x^%d", coefficients[i], i);
        } else if (i > 1) {
            printf(" + %.2f*x^%d", coefficients[i], i);
        } else if (i == 1) {
            printf(" + %.2f*x", coefficients[i]);
        } else {
            printf(" + %.2f", coefficients[i]);
        }
    }
    
    printf("\nResult at x = %.2f: %.6f\n", x, result);
    
    double derivative = 0.0;
    if (degree > 0) {
        for (int i = 1; i <= degree; i++) {
            derivative += coefficients[i] * i * pow(x, i - 1);
        }
        printf("Derivative at x = %.2f: %.6f\n", x, derivative);
    }
    
    return 0;
}