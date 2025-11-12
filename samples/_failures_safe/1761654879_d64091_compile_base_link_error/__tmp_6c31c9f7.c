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
        
        if (sscanf(input, "%lf", &coefficients[i]) != 1) {
            fprintf(stderr, "Invalid coefficient format\n");
            return 1;
        }
    }
    
    printf("\nFinding roots using bisection method...\n");
    printf("Enter search interval [a, b]:\n");
    
    double a, b;
    printf("a: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading a\n");
        return 1;
    }
    if (sscanf(input, "%lf", &a) != 1) {
        fprintf(stderr, "Invalid a format\n");
        return 1;
    }
    
    printf("b: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading b\n");
        return 1;
    }
    if (sscanf(input, "%lf", &b) != 1) {
        fprintf(stderr, "Invalid b format\n");
        return 1;
    }
    
    if (a >= b) {
        fprintf(stderr, "a must be less than b\n");
        return 1;
    }
    
    double fa = calculate_polynomial(a, coefficients, degree);
    double fb = calculate_polynomial(b, coefficients, degree);
    
    if (fa * fb > 0) {
        fprintf(stderr, "No sign change in interval, cannot guarantee root\n");
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
    
    printf("\nResults:\n");
    printf("Approximate root: %.10f\n", root);
    printf("Function value at root: %.10f\n", calculate_polynomial(root, coefficients, degree));
    printf("Iterations: %d\n", iterations);
    
    return 0;
}