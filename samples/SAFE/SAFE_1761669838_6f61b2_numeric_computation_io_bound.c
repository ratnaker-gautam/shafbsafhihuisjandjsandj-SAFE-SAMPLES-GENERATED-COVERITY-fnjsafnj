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
    printf("\n\n");
    
    printf("Finding roots using bisection method...\n");
    
    const int max_iterations = 100;
    const double tolerance = 1e-10;
    int roots_found = 0;
    
    for (double x_start = -10.0; x_start <= 10.0 && roots_found < degree; x_start += 0.5) {
        double x_low = x_start;
        double x_high = x_start + 0.5;
        
        double f_low = calculate_polynomial(x_low, coefficients, degree);
        double f_high = calculate_polynomial(x_high, coefficients, degree);
        
        if (f_low * f_high > 0) {
            continue;
        }
        
        double root = 0.0;
        int iterations = 0;
        
        while (iterations < max_iterations) {
            root = (x_low + x_high) / 2.0;
            double f_root = calculate_polynomial(root, coefficients, degree);
            
            if (fabs(f_root) < tolerance) {
                break;
            }
            
            if (f_low * f_root < 0) {
                x_high = root;
                f_high = f_root;
            } else {
                x_low = root;
                f_low = f_root;
            }
            
            iterations++;
        }
        
        if (iterations < max_iterations) {
            printf("Root %d: x = %.6f (found in %d iterations)\n", 
                   roots_found + 1, root, iterations);
            roots_found++;
        }
    }
    
    if (roots_found == 0) {
        printf("No roots found in the search interval [-10, 10]\n");
    } else {
        printf("Found %d root(s) out of maximum %d\n", roots_found, degree);
    }
    
    printf("\nEvaluating polynomial at sample points:\n");
    for (double x = -2.0; x <= 2.0; x += 0.5) {
        double value = calculate_polynomial(x, coefficients, degree);
        printf("f(%.1f) = %.6f\n", x, value);
    }
    
    return 0;
}