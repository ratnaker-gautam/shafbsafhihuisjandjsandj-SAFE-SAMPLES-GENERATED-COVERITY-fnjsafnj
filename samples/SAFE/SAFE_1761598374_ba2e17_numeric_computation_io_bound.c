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

int read_coefficients(double coeffs[], int max_degree) {
    char input[MAX_INPUT_LEN];
    int degree = -1;
    
    printf("Enter polynomial degree (0-10): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    if (sscanf(input, "%d", &degree) != 1) {
        return -1;
    }
    
    if (degree < 0 || degree > 10) {
        return -1;
    }
    
    printf("Enter %d coefficients (from constant term to highest degree):\n", degree + 1);
    for (int i = 0; i <= degree; i++) {
        printf("Coefficient %d: ", i);
        if (fgets(input, sizeof(input), stdin) == NULL) {
            return -1;
        }
        
        if (sscanf(input, "%lf", &coeffs[i]) != 1) {
            return -1;
        }
    }
    
    return degree;
}

int main(void) {
    double coefficients[11];
    char input[MAX_INPUT_LEN];
    double x_value;
    int degree;
    
    printf("Polynomial Calculator\n");
    printf("====================\n\n");
    
    degree = read_coefficients(coefficients, 10);
    if (degree < 0) {
        fprintf(stderr, "Error: Invalid polynomial coefficients\n");
        return EXIT_FAILURE;
    }
    
    printf("\nEnter x value for evaluation: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error: Failed to read input\n");
        return EXIT_FAILURE;
    }
    
    if (sscanf(input, "%lf", &x_value) != 1) {
        fprintf(stderr, "Error: Invalid x value\n");
        return EXIT_FAILURE;
    }
    
    if (!isfinite(x_value)) {
        fprintf(stderr, "Error: x value is not finite\n");
        return EXIT_FAILURE;
    }
    
    double result = calculate_polynomial(x_value, coefficients, degree);
    
    if (!isfinite(result)) {
        fprintf(stderr, "Error: Result is not finite\n");
        return EXIT_FAILURE;
    }
    
    printf("\nPolynomial: ");
    for (int i = 0; i <= degree; i++) {
        if (i == 0) {
            printf("%.2f", coefficients[i]);
        } else if (i == 1) {
            printf(" + %.2fx", coefficients[i]);
        } else {
            printf(" + %.2fx^%d", coefficients[i], i);
        }
    }
    
    printf("\nP(%.2f) = %.6f\n", x_value, result);
    
    printf("\nGenerating evaluation table for x = -2.0 to 2.0 in steps of 0.5:\n");
    printf("x\t\tP(x)\n");
    printf("-----------------------\n");
    
    for (double x = -2.0; x <= 2.0; x += 0.5) {
        double y = calculate_polynomial(x, coefficients, degree);
        if (isfinite(y)) {
            printf("%.1f\t\t%.6f\n", x, y);
        } else {
            printf("%.1f\t\toverflow\n", x);
        }
    }
    
    return EXIT_SUCCESS;
}