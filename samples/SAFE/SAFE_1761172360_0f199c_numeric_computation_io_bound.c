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
    double x_power = 1.0;
    
    for (int i = 0; i <= degree; i++) {
        result += coeffs[i] * x_power;
        x_power *= x;
    }
    
    return result;
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 1];
    int degree;
    double x_value;
    
    printf("Polynomial Calculator\n");
    printf("Enter polynomial degree (0-10): ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (sscanf(input_buffer, "%d", &degree) != 1) {
        fprintf(stderr, "Invalid degree format\n");
        return 1;
    }
    
    if (degree < 0 || degree > 10) {
        fprintf(stderr, "Degree must be between 0 and 10\n");
        return 1;
    }
    
    double coefficients[11] = {0};
    
    for (int i = 0; i <= degree; i++) {
        printf("Enter coefficient for x^%d: ", i);
        
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            fprintf(stderr, "Error reading input\n");
            return 1;
        }
        
        if (sscanf(input_buffer, "%lf", &coefficients[i]) != 1) {
            fprintf(stderr, "Invalid coefficient format\n");
            return 1;
        }
    }
    
    printf("Enter x value to evaluate: ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (sscanf(input_buffer, "%lf", &x_value) != 1) {
        fprintf(stderr, "Invalid x value format\n");
        return 1;
    }
    
    double result = calculate_polynomial(x_value, coefficients, degree);
    
    printf("Polynomial: ");
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
    
    printf("\nResult at x = %.2f: %.6f\n", x_value, result);
    
    printf("Generating sample points...\n");
    for (int i = 0; i <= 10; i++) {
        double sample_x = -2.0 + i * 0.4;
        double sample_result = calculate_polynomial(sample_x, coefficients, degree);
        printf("f(%.2f) = %.6f\n", sample_x, sample_result);
    }
    
    return 0;
}