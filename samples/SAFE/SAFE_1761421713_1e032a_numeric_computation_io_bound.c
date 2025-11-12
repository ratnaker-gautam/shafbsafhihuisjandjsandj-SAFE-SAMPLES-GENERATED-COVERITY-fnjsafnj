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
    double x_power = 1.0;
    
    for (int i = 0; i <= degree; i++) {
        result += coeffs[i] * x_power;
        x_power *= x;
    }
    
    return result;
}

int is_valid_double(const char* str) {
    char* endptr;
    strtod(str, &endptr);
    return endptr != str && *endptr == '\0';
}

int main(void) {
    char input[MAX_INPUT_LEN];
    int degree;
    
    printf("Enter polynomial degree (0-10): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
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
        printf("Coefficient %d: ", i);
        if (fgets(input, sizeof(input), stdin) == NULL) {
            fprintf(stderr, "Input error\n");
            return 1;
        }
        
        if (!is_valid_double(input)) {
            fprintf(stderr, "Invalid coefficient format\n");
            return 1;
        }
        
        coefficients[i] = strtod(input, NULL);
    }
    
    printf("Enter x value for evaluation: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    if (!is_valid_double(input)) {
        fprintf(stderr, "Invalid x value format\n");
        return 1;
    }
    
    double x = strtod(input, NULL);
    
    double result = compute_polynomial(x, coefficients, degree);
    
    printf("Polynomial coefficients: ");
    for (int i = 0; i <= degree; i++) {
        printf("%.3f", coefficients[i]);
        if (i < degree) printf(", ");
    }
    printf("\n");
    
    printf("P(%.3f) = %.6f\n", x, result);
    
    printf("Evaluating polynomial at multiple points...\n");
    printf("x\t\tP(x)\n");
    printf("-------------------\n");
    
    for (int i = -5; i <= 5; i++) {
        double eval_x = i * 0.5;
        double eval_result = compute_polynomial(eval_x, coefficients, degree);
        printf("%.1f\t\t%.6f\n", eval_x, eval_result);
    }
    
    return 0;
}