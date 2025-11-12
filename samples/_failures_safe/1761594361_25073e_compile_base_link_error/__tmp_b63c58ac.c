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

int main(void) {
    int degree;
    double x_start, x_end, step;
    int num_points;
    
    printf("Polynomial Function Evaluator\n");
    printf("=============================\n\n");
    
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
        char prompt[50];
        snprintf(prompt, sizeof(prompt), "Coefficient for x^%d: ", i);
        if (!read_double(prompt, &coefficients[i])) {
            fprintf(stderr, "Error: Invalid coefficient input\n");
            return 1;
        }
    }
    
    if (!read_double("Enter start value for x: ", &x_start)) {
        fprintf(stderr, "Error: Invalid start value\n");
        return 1;
    }
    
    if (!read_double("Enter end value for x: ", &x_end)) {
        fprintf(stderr, "Error: Invalid end value\n");
        return 1;
    }
    
    if (x_end < x_start) {
        fprintf(stderr, "Error: End value must be greater than or equal to start value\n");
        return 1;
    }
    
    if (!read_double("Enter step size: ", &step)) {
        fprintf(stderr, "Error: Invalid step size\n");
        return 1;
    }
    
    if (step <= 0.0) {
        fprintf(stderr, "Error: Step size must be positive\n");
        return 1;
    }
    
    num_points = (int)((x_end - x_start) / step) + 1;
    if (num_points <= 0 || num_points > 10000) {
        fprintf(stderr, "Error: Too many or too few points to evaluate\n");
        return 1;
    }
    
    printf("\nEvaluating polynomial: ");
    for (int i = degree; i >= 0; i--) {
        if (coefficients[i] != 0.0) {
            if (i == degree) {
                printf("%.2f", coefficients[i]);
            } else {
                printf(" %c %.2f", coefficients[i] >= 0 ? '+' : '-', fabs(coefficients[i]));
            }
            if (i > 0) {
                printf("x");
                if (i > 1) {
                    printf("^%d", i);
                }
            }
        }
    }
    printf("\n\n");
    
    printf(" x\t\t f(x)\n");
    printf("-----\t\t-----\n");
    
    double x = x_start;
    for (int i = 0; i < num_points; i++) {
        if (x > x_end + 1e-10) {
            break;
        }
        
        double result = calculate_polynomial(x, coefficients, degree);
        if (!isfinite(result)) {
            fprintf(stderr, "Warning: Result is not finite at x = %.2f\n", x);
        } else {
            printf("%.2f\t\t%.6f\n", x, result);
        }
        
        x += step;
        if (x < x_start) {
            break;
        }
    }
    
    return 0;
}