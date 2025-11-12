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
    
    printf("Enter x value for evaluation: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (sscanf(input, "%lf", &x) != 1) {
        fprintf(stderr, "Invalid x value format\n");
        return 1;
    }
    
    if (!isfinite(x)) {
        fprintf(stderr, "Invalid x value\n");
        return 1;
    }
    
    double result = calculate_polynomial(x, coefficients, degree);
    
    printf("Polynomial value at x = %.6f: %.6f\n", x, result);
    
    printf("Performing root approximation...\n");
    
    double left = -10.0;
    double right = 10.0;
    const int max_iterations = 100;
    const double tolerance = 1e-10;
    
    for (int iter = 0; iter < max_iterations; iter++) {
        double mid = (left + right) / 2.0;
        double f_mid = calculate_polynomial(mid, coefficients, degree);
        
        if (fabs(f_mid) < tolerance) {
            printf("Found approximate root: %.6f\n", mid);
            break;
        }
        
        double f_left = calculate_polynomial(left, coefficients, degree);
        
        if (f_mid * f_left < 0) {
            right = mid;
        } else {
            left = mid;
        }
        
        if (iter == max_iterations - 1) {
            printf("No root found in range [-10, 10] within tolerance\n");
        }
    }
    
    printf("Computing polynomial table from -2 to 2...\n");
    
    for (double table_x = -2.0; table_x <= 2.0; table_x += 0.5) {
        double table_y = calculate_polynomial(table_x, coefficients, degree);
        printf("f(%.1f) = %.6f\n", table_x, table_y);
    }
    
    return 0;
}