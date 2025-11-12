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

int is_valid_double(const char* str) {
    char* endptr;
    strtod(str, &endptr);
    return endptr != str && *endptr == '\0';
}

int main(void) {
    char input[MAX_INPUT_LEN];
    int degree;
    
    printf("Polynomial Root Approximation Calculator\n");
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
        
        input[strcspn(input, "\n")] = '\0';
        
        if (!is_valid_double(input)) {
            fprintf(stderr, "Invalid coefficient format\n");
            return 1;
        }
        
        coefficients[i] = strtod(input, NULL);
    }
    
    printf("Enter search interval [a,b] for root finding:\n");
    printf("a: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (!is_valid_double(input)) {
        fprintf(stderr, "Invalid a value\n");
        return 1;
    }
    
    double a = strtod(input, NULL);
    
    printf("b: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (!is_valid_double(input)) {
        fprintf(stderr, "Invalid b value\n");
        return 1;
    }
    
    double b = strtod(input, NULL);
    
    if (a >= b) {
        fprintf(stderr, "a must be less than b\n");
        return 1;
    }
    
    double fa = calculate_polynomial(a, coefficients, degree);
    double fb = calculate_polynomial(b, coefficients, degree);
    
    if (fa * fb > 0) {
        fprintf(stderr, "Function values at endpoints have same sign, no root guaranteed\n");
        return 1;
    }
    
    const int max_iterations = 100;
    const double tolerance = 1e-10;
    double root = 0.0;
    int iterations = 0;
    
    for (iterations = 0; iterations < max_iterations; iterations++) {
        double mid = (a + b) / 2.0;
        double fmid = calculate_polynomial(mid, coefficients, degree);
        
        if (fabs(fmid) < tolerance || (b - a) / 2.0 < tolerance) {
            root = mid;
            break;
        }
        
        if (fa * fmid < 0) {
            b = mid;
            fb = fmid;
        } else {
            a = mid;
            fa = fmid;
        }
    }
    
    printf("Approximate root found: %.10f\n", root);
    printf("Function value at root: %.10f\n", calculate_polynomial(root, coefficients, degree));
    printf("Iterations: %d\n", iterations);
    
    return 0;
}