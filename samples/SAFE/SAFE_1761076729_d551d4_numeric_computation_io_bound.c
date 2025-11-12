//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_LEN 256

double calculate_polynomial(double x, double coeffs[], int degree) {
    double result = 0.0;
    double term = 1.0;
    
    for (int i = 0; i <= degree; i++) {
        result += coeffs[i] * term;
        term *= x;
    }
    
    return result;
}

int main(void) {
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
    
    printf("Enter search range (min max): ");
    double min_x, max_x;
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading range\n");
        return 1;
    }
    
    if (sscanf(input, "%lf %lf", &min_x, &max_x) != 2) {
        fprintf(stderr, "Invalid range format\n");
        return 1;
    }
    
    if (min_x >= max_x) {
        fprintf(stderr, "Invalid range: min must be less than max\n");
        return 1;
    }
    
    printf("Enter step size: ");
    double step;
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading step\n");
        return 1;
    }
    
    if (sscanf(input, "%lf", &step) != 1) {
        fprintf(stderr, "Invalid step format\n");
        return 1;
    }
    
    if (step <= 0.0) {
        fprintf(stderr, "Step must be positive\n");
        return 1;
    }
    
    printf("\nSearching for roots in [%.3f, %.3f] with step %.3f\n", min_x, max_x, step);
    printf("Root approximations found:\n");
    
    int root_count = 0;
    double prev_value = calculate_polynomial(min_x, coefficients, degree);
    
    for (double x = min_x + step; x <= max_x; x += step) {
        double curr_value = calculate_polynomial(x, coefficients, degree);
        
        if (prev_value * curr_value <= 0.0) {
            double root_approx = (prev_value == 0.0) ? x - step : 
                               (curr_value == 0.0) ? x : 
                               (x - step + x) / 2.0;
            
            printf("Root near x = %.6f (f(x) = %.6f)\n", root_approx, 
                   calculate_polynomial(root_approx, coefficients, degree));
            root_count++;
        }
        
        prev_value = curr_value;
    }
    
    if (root_count == 0) {
        printf("No roots found in the specified range.\n");
    } else {
        printf("Total roots found: %d\n", root_count);
    }
    
    return 0;
}