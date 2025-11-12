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

int main() {
    char input[MAX_INPUT_LEN];
    int degree;
    
    printf("Polynomial Calculator\n");
    printf("Enter polynomial degree (0-10): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (sscanf(input, "%d", &degree) != 1) {
        fprintf(stderr, "Invalid degree format\n");
        return 1;
    }
    
    if (degree < 0 || degree > 10) {
        fprintf(stderr, "Degree must be between 0 and 10\n");
        return 1;
    }
    
    double coefficients[11];
    printf("Enter %d coefficients (from constant term to highest degree):\n", degree + 1);
    
    for (int i = 0; i <= degree; i++) {
        printf("Coefficient for x^%d: ", i);
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            fprintf(stderr, "Error reading coefficient\n");
            return 1;
        }
        
        if (sscanf(input, "%lf", &coefficients[i]) != 1) {
            fprintf(stderr, "Invalid coefficient format\n");
            return 1;
        }
    }
    
    printf("Enter x value to evaluate: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading x value\n");
        return 1;
    }
    
    double x;
    if (sscanf(input, "%lf", &x) != 1) {
        fprintf(stderr, "Invalid x value format\n");
        return 1;
    }
    
    double result = calculate_polynomial(x, coefficients, degree);
    
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
    printf("\n");
    
    printf("P(%.2f) = %.6f\n", x, result);
    
    printf("Enter step size for table (0.1-5.0): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading step size\n");
        return 1;
    }
    
    double step;
    if (sscanf(input, "%lf", &step) != 1) {
        fprintf(stderr, "Invalid step size format\n");
        return 1;
    }
    
    if (step < 0.1 || step > 5.0) {
        fprintf(stderr, "Step size must be between 0.1 and 5.0\n");
        return 1;
    }
    
    printf("Enter number of steps (1-20): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading number of steps\n");
        return 1;
    }
    
    int num_steps;
    if (sscanf(input, "%d", &num_steps) != 1) {
        fprintf(stderr, "Invalid number of steps format\n");
        return 1;
    }
    
    if (num_steps < 1 || num_steps > 20) {
        fprintf(stderr, "Number of steps must be between 1 and 20\n");
        return 1;
    }
    
    printf("\nFunction table:\n");
    printf("x\t\tP(x)\n");
    printf("-------------------\n");
    
    double current_x = x;
    for (int i = 0; i < num_steps; i++) {
        double value = calculate_polynomial(current_x, coefficients, degree);
        printf("%.2f\t\t%.6f\n", current_x, value);
        current_x += step;
    }
    
    return 0;
}