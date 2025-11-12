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

int read_double(const char* prompt, double* value) {
    char buffer[MAX_INPUT_LEN];
    char* endptr;
    
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    
    buffer[strcspn(buffer, "\n")] = '\0';
    *value = strtod(buffer, &endptr);
    
    if (endptr == buffer || *endptr != '\0') {
        return 0;
    }
    
    return 1;
}

int read_int(const char* prompt, int* value) {
    char buffer[MAX_INPUT_LEN];
    char* endptr;
    long temp;
    
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    
    buffer[strcspn(buffer, "\n")] = '\0';
    temp = strtol(buffer, &endptr, 10);
    
    if (endptr == buffer || *endptr != '\0') {
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
    double coefficients[101];
    int degree;
    
    printf("Polynomial Calculator\n");
    printf("====================\n\n");
    
    if (!read_double("Enter value for x: ", &x)) {
        fprintf(stderr, "Error: Invalid input for x\n");
        return 1;
    }
    
    if (!read_int("Enter polynomial degree (0-100): ", &degree)) {
        fprintf(stderr, "Error: Invalid degree\n");
        return 1;
    }
    
    printf("\nEnter coefficients from constant term to x^%d:\n", degree);
    for (int i = 0; i <= degree; i++) {
        char prompt[50];
        snprintf(prompt, sizeof(prompt), "Coefficient for x^%d: ", i);
        
        if (!read_double(prompt, &coefficients[i])) {
            fprintf(stderr, "Error: Invalid coefficient\n");
            return 1;
        }
    }
    
    double result = calculate_polynomial(x, coefficients, degree);
    
    printf("\nPolynomial evaluation result:\n");
    printf("P(%g) = ", x);
    
    int first_term = 1;
    for (int i = 0; i <= degree; i++) {
        if (coefficients[i] != 0.0) {
            if (!first_term && coefficients[i] > 0) {
                printf(" + ");
            } else if (coefficients[i] < 0) {
                printf(" - ");
            }
            
            double abs_coeff = fabs(coefficients[i]);
            
            if (i == 0) {
                printf("%g", abs_coeff);
            } else if (i == 1) {
                if (abs_coeff != 1.0) {
                    printf("%g*x", abs_coeff);
                } else {
                    printf("x");
                }
            } else {
                if (abs_coeff != 1.0) {
                    printf("%g*x^%d", abs_coeff, i);
                } else {
                    printf("x^%d", i);
                }
            }
            
            first_term = 0;
        }
    }
    
    if (first_term) {
        printf("0");
    }
    
    printf("\n\nNumerical result: %g\n", result);
    
    return 0;
}