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

int parse_double(const char *str, double *value) {
    char *endptr;
    *value = strtod(str, &endptr);
    return (*endptr == '\0' || *endptr == '\n');
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    int degree;
    
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
        printf("Coefficient %d: ", i);
        if (fgets(input, sizeof(input), stdin) == NULL) {
            fprintf(stderr, "Error reading coefficient\n");
            return 1;
        }
        
        if (!parse_double(input, &coefficients[i])) {
            fprintf(stderr, "Invalid coefficient format\n");
            return 1;
        }
    }
    
    printf("Enter x value to evaluate polynomial: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading x value\n");
        return 1;
    }
    
    double x;
    if (!parse_double(input, &x)) {
        fprintf(stderr, "Invalid x value format\n");
        return 1;
    }
    
    double result = compute_polynomial(x, coefficients, degree);
    
    printf("Polynomial coefficients: ");
    for (int i = 0; i <= degree; i++) {
        printf("%.2f", coefficients[i]);
        if (i < degree) printf(", ");
    }
    printf("\n");
    
    printf("P(%.2f) = %.6f\n", x, result);
    
    printf("Evaluating polynomial at 10 points from x=0 to x=10:\n");
    for (int i = 0; i <= 10; i++) {
        double eval_x = i;
        double eval_result = compute_polynomial(eval_x, coefficients, degree);
        printf("P(%.1f) = %.6f\n", eval_x, eval_result);
    }
    
    return 0;
}