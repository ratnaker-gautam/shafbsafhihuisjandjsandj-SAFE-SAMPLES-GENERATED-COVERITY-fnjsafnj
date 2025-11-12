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

int is_valid_double(const char* str, double* value) {
    char* endptr;
    *value = strtod(str, &endptr);
    return (endptr != str && *endptr == '\0');
}

int main() {
    char input[MAX_INPUT_LEN];
    int degree;
    
    printf("Polynomial Root Finder\n");
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
        
        double coeff;
        if (!is_valid_double(input, &coeff)) {
            fprintf(stderr, "Invalid coefficient format\n");
            return 1;
        }
        
        coefficients[i] = coeff;
    }
    
    printf("Enter search interval [a,b]:\n");
    double a, b;
    
    printf("a: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    input[strcspn(input, "\n")] = '\0';
    if (!is_valid_double(input, &a)) {
        fprintf(stderr, "Invalid value for a\n");
        return 1;
    }
    
    printf("b: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    input[strcspn(input, "\n")] = '\0';
    if (!is_valid_double(input, &b)) {
        fprintf(stderr, "Invalid value for b\n");
        return 1;
    }
    
    if (a >= b) {
        fprintf(stderr, "a must be less than b\n");
        return 1;
    }
    
    printf("Enter tolerance (e.g., 1e-6): ");
    double tolerance;
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    input[strcspn(input, "\n")] = '\0';
    if (!is_valid_double(input, &tolerance)) {
        fprintf(stderr, "Invalid tolerance format\n");
        return 1;
    }
    
    if (tolerance <= 0.0 || tolerance >= 1.0) {
        fprintf(stderr, "Tolerance must be between 0 and 1\n");
        return 1;
    }
    
    double fa = calculate_polynomial(a, coefficients, degree);
    double fb = calculate_polynomial(b, coefficients, degree);
    
    if (fa * fb > 0) {
        printf("No root found in the interval [%.6f, %.6f]\n", a, b);
        return 0;
    }
    
    int max_iterations = 100;
    double root = 0.0;
    
    for (int iter = 0; iter < max_iterations; iter++) {
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
        
        if (iter == max_iterations - 1) {
            root = (a + b) / 2.0;
        }
    }
    
    printf("Approximate root found at x = %.8f\n", root);
    printf("f(%.8f) = %.8f\n", root, calculate_polynomial(root, coefficients, degree));
    
    return 0;
}