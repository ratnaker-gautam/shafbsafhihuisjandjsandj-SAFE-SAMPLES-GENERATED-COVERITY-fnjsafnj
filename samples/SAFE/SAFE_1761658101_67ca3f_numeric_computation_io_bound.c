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
    char input[MAX_INPUT_LEN];
    int degree;
    double x;
    
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
    
    double coefficients[11] = {0};
    
    for (int i = 0; i <= degree; i++) {
        printf("Enter coefficient for x^%d: ", i);
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            fprintf(stderr, "Error reading input\n");
            return 1;
        }
        
        if (sscanf(input, "%lf", &coefficients[i]) != 1) {
            fprintf(stderr, "Invalid coefficient format\n");
            return 1;
        }
    }
    
    printf("Enter x value to evaluate: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (sscanf(input, "%lf", &x) != 1) {
        fprintf(stderr, "Invalid x value format\n");
        return 1;
    }
    
    if (!isfinite(x)) {
        fprintf(stderr, "x value must be finite\n");
        return 1;
    }
    
    double result = calculate_polynomial(x, coefficients, degree);
    
    if (!isfinite(result)) {
        fprintf(stderr, "Result is not finite\n");
        return 1;
    }
    
    printf("Polynomial: ");
    for (int i = degree; i >= 0; i--) {
        if (coefficients[i] != 0) {
            if (i == degree) {
                printf("%.2f", coefficients[i]);
            } else {
                printf(" %c %.2f", coefficients[i] >= 0 ? '+' : '-', fabs(coefficients[i]));
            }
            
            if (i > 1) {
                printf("x^%d", i);
            } else if (i == 1) {
                printf("x");
            }
        }
    }
    printf("\n");
    
    printf("P(%.2f) = %.6f\n", x, result);
    
    printf("Evaluating at multiple points:\n");
    for (double test_x = -2.0; test_x <= 2.0; test_x += 0.5) {
        double test_result = calculate_polynomial(test_x, coefficients, degree);
        if (isfinite(test_result)) {
            printf("P(%.1f) = %.4f\n", test_x, test_result);
        }
    }
    
    return 0;
}