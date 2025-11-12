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
    for (int i = degree; i >= 0; i--) {
        result = result * x + coeffs[i];
    }
    return result;
}

int read_double(const char *prompt, double *value) {
    char buffer[MAX_INPUT_LEN];
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    char *endptr;
    *value = strtod(buffer, &endptr);
    if (endptr == buffer || *endptr != '\n') {
        return 0;
    }
    return 1;
}

int read_int(const char *prompt, int *value) {
    char buffer[MAX_INPUT_LEN];
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    char *endptr;
    long temp = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n' || temp < INT_MIN || temp > INT_MAX) {
        return 0;
    }
    *value = (int)temp;
    return 1;
}

int main(void) {
    int degree;
    double x;
    double result;
    
    printf("Polynomial Evaluator\n");
    printf("===================\n");
    
    if (!read_int("Enter polynomial degree (0-10): ", &degree)) {
        fprintf(stderr, "Error: Invalid degree input\n");
        return 1;
    }
    
    if (degree < 0 || degree > 10) {
        fprintf(stderr, "Error: Degree must be between 0 and 10\n");
        return 1;
    }
    
    double coefficients[11] = {0};
    
    printf("Enter coefficients from constant term to highest degree:\n");
    for (int i = 0; i <= degree; i++) {
        char prompt[64];
        snprintf(prompt, sizeof(prompt), "Coefficient for x^%d: ", i);
        if (!read_double(prompt, &coefficients[i])) {
            fprintf(stderr, "Error: Invalid coefficient input\n");
            return 1;
        }
    }
    
    if (!read_double("Enter value of x to evaluate: ", &x)) {
        fprintf(stderr, "Error: Invalid x value input\n");
        return 1;
    }
    
    result = compute_polynomial(x, coefficients, degree);
    
    printf("\nPolynomial: ");
    int first_term = 1;
    for (int i = degree; i >= 0; i--) {
        if (coefficients[i] != 0.0) {
            if (!first_term) {
                printf(" + ");
            }
            if (i == 0) {
                printf("%.2f", coefficients[i]);
            } else if (i == 1) {
                printf("%.2fx", coefficients[i]);
            } else {
                printf("%.2fx^%d", coefficients[i], i);
            }
            first_term = 0;
        }
    }
    if (first_term) {
        printf("0");
    }
    
    printf("\nResult at x = %.2f: %.6f\n", x, result);
    
    printf("\nGenerating evaluation table...\n");
    printf("x\t\tf(x)\n");
    printf("----------------------\n");
    
    for (int i = -5; i <= 5; i++) {
        double table_x = i * 0.5;
        double table_result = compute_polynomial(table_x, coefficients, degree);
        printf("%.1f\t\t%.6f\n", table_x, table_result);
    }
    
    return 0;
}