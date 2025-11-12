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
    
    *value = strtod(buffer, &endptr);
    if (endptr == buffer || *endptr != '\n') {
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
    
    temp = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n') {
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
    double x;
    double result;
    
    printf("Polynomial Calculator\n");
    printf("====================\n\n");
    
    if (!read_int("Enter polynomial degree (0-10): ", &degree)) {
        fprintf(stderr, "Error: Invalid degree input\n");
        return 1;
    }
    
    if (degree < 0 || degree > 10) {
        fprintf(stderr, "Error: Degree must be between 0 and 10\n");
        return 1;
    }
    
    double coefficients[11] = {0};
    
    printf("Enter coefficients for x^0 to x^%d:\n", degree);
    for (int i = 0; i <= degree; i++) {
        char prompt[32];
        snprintf(prompt, sizeof(prompt), "Coefficient for x^%d: ", i);
        
        if (!read_double(prompt, &coefficients[i])) {
            fprintf(stderr, "Error: Invalid coefficient input\n");
            return 1;
        }
    }
    
    if (!read_double("Enter value of x: ", &x)) {
        fprintf(stderr, "Error: Invalid x value input\n");
        return 1;
    }
    
    if (fabs(x) > 1e100) {
        fprintf(stderr, "Error: x value too large\n");
        return 1;
    }
    
    result = calculate_polynomial(x, coefficients, degree);
    
    if (!isfinite(result)) {
        fprintf(stderr, "Error: Result is not finite\n");
        return 1;
    }
    
    printf("\nPolynomial: ");
    for (int i = degree; i >= 0; i--) {
        if (i == degree) {
            printf("%.2fx^%d", coefficients[i], i);
        } else if (i > 1) {
            printf(" + %.2fx^%d", coefficients[i], i);
        } else if (i == 1) {
            printf(" + %.2fx", coefficients[i]);
        } else {
            printf(" + %.2f", coefficients[i]);
        }
    }
    
    printf("\nResult for x = %.2f: %.6f\n", x, result);
    
    return 0;
}