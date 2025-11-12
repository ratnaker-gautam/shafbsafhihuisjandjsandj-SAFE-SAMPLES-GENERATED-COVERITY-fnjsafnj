//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define MAX_INPUT_LEN 256

double compute_polynomial(double x, double coeffs[], int degree) {
    double result = 0.0;
    double power = 1.0;
    
    for (int i = 0; i <= degree; i++) {
        result += coeffs[i] * power;
        power *= x;
    }
    
    return result;
}

int read_coefficients(double coeffs[], int max_degree) {
    char input[MAX_INPUT_LEN];
    char *token;
    int count = 0;
    
    printf("Enter polynomial coefficients (space separated, highest degree first): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    token = strtok(input, " \t\n");
    while (token != NULL && count <= max_degree) {
        char *endptr;
        double value = strtod(token, &endptr);
        
        if (endptr == token || *endptr != '\0') {
            return -1;
        }
        
        if (!isfinite(value)) {
            return -1;
        }
        
        coeffs[count] = value;
        count++;
        token = strtok(NULL, " \t\n");
    }
    
    return count - 1;
}

int main(void) {
    double coefficients[10];
    int degree;
    char input[MAX_INPUT_LEN];
    double x_value;
    
    printf("Polynomial Evaluator\n");
    printf("===================\n\n");
    
    degree = read_coefficients(coefficients, 9);
    if (degree < 0) {
        fprintf(stderr, "Error: Invalid coefficients input\n");
        return 1;
    }
    
    printf("Polynomial degree: %d\n", degree);
    printf("Enter x value to evaluate: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error: Failed to read input\n");
        return 1;
    }
    
    char *endptr;
    x_value = strtod(input, &endptr);
    if (endptr == input || *endptr != '\n') {
        fprintf(stderr, "Error: Invalid x value\n");
        return 1;
    }
    
    if (!isfinite(x_value)) {
        fprintf(stderr, "Error: x value is not finite\n");
        return 1;
    }
    
    double result = compute_polynomial(x_value, coefficients, degree);
    
    if (!isfinite(result)) {
        fprintf(stderr, "Error: Result is not finite\n");
        return 1;
    }
    
    printf("P(%.6f) = %.6f\n", x_value, result);
    
    printf("\nEvaluating at multiple points:\n");
    for (int i = -2; i <= 2; i++) {
        double test_x = i * 0.5;
        double test_result = compute_polynomial(test_x, coefficients, degree);
        
        if (isfinite(test_result)) {
            printf("P(%.1f) = %.6f\n", test_x, test_result);
        }
    }
    
    return 0;
}