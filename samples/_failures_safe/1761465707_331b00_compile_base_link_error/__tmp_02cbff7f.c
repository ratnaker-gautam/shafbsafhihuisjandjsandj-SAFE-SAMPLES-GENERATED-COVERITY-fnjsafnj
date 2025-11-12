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
    for (int i = 0; i <= degree; i++) {
        result += coeffs[i] * pow(x, i);
    }
    return result;
}

int main() {
    char input[MAX_INPUT_LEN];
    int degree;
    
    printf("Polynomial Calculator\n");
    printf("Enter polynomial degree (1-10): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (sscanf(input, "%d", &degree) != 1) {
        fprintf(stderr, "Invalid degree format\n");
        return 1;
    }
    
    if (degree < 1 || degree > 10) {
        fprintf(stderr, "Degree must be between 1 and 10\n");
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
    double x;
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading x value\n");
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
    
    double result = compute_polynomial(x, coefficients, degree);
    
    if (!isfinite(result)) {
        fprintf(stderr, "Result is not finite (overflow or invalid computation)\n");
        return 1;
    }
    
    printf("Polynomial: ");
    for (int i = 0; i <= degree; i++) {
        if (i == 0) {
            printf("%.2f", coefficients[i]);
        } else {
            printf(" + %.2fx", coefficients[i]);
            if (i > 1) printf("^%d", i);
        }
    }
    printf("\n");
    
    printf("P(%.2f) = %.6f\n", x, result);
    
    printf("Enter step size for table (0.1-5.0): ");
    double step;
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading step size\n");
        return 1;
    }
    
    if (sscanf(input, "%lf", &step) != 1) {
        fprintf(stderr, "Invalid step format\n");
        return 1;
    }
    
    if (step < 0.1 || step > 5.0) {
        fprintf(stderr, "Step must be between 0.1 and 5.0\n");
        return 1;
    }
    
    printf("Enter number of points (2-20): ");
    int points;
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading points count\n");
        return 1;
    }
    
    if (sscanf(input, "%d", &points) != 1) {
        fprintf(stderr, "Invalid points format\n");
        return 1;
    }
    
    if (points < 2 || points > 20) {
        fprintf(stderr, "Points must be between 2 and 20\n");
        return 1;
    }
    
    printf("\nFunction table:\n");
    printf("x\t\tP(x)\n");
    printf("-------------------\n");
    
    for (int i = 0; i < points; i++) {
        double current_x = x + i * step;
        if (!isfinite(current_x)) {
            fprintf(stderr, "x value overflow\n");
            return 1;
        }
        
        double current_result = compute_polynomial(current_x, coefficients, degree);
        if (!isfinite(current_result)) {
            fprintf(stderr, "Result overflow at x=%.2f\n", current_x);
            return 1;
        }
        
        printf("%.2f\t\t%.6f\n", current_x, current_result);
    }
    
    return 0;
}