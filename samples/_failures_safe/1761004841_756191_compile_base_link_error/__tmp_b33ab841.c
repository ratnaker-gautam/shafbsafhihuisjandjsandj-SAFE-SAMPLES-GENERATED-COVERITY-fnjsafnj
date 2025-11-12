//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_LEN 256

double calculate_polynomial(double x, double coefficients[], int degree) {
    double result = 0.0;
    for (int i = 0; i <= degree; i++) {
        result += coefficients[i] * pow(x, i);
    }
    return result;
}

int main() {
    char input[MAX_INPUT_LEN];
    int degree;
    
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
    
    double result = calculate_polynomial(x, coefficients, degree);
    printf("P(%g) = %g\n", x, result);
    
    printf("Enter range start: ");
    double start;
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading range start\n");
        return 1;
    }
    
    if (sscanf(input, "%lf", &start) != 1) {
        fprintf(stderr, "Invalid range start format\n");
        return 1;
    }
    
    printf("Enter range end: ");
    double end;
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading range end\n");
        return 1;
    }
    
    if (sscanf(input, "%lf", &end) != 1) {
        fprintf(stderr, "Invalid range end format\n");
        return 1;
    }
    
    if (start >= end) {
        fprintf(stderr, "Range start must be less than range end\n");
        return 1;
    }
    
    printf("Enter number of points to evaluate: ");
    int points;
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading number of points\n");
        return 1;
    }
    
    if (sscanf(input, "%d", &points) != 1) {
        fprintf(stderr, "Invalid number of points format\n");
        return 1;
    }
    
    if (points <= 0 || points > 1000) {
        fprintf(stderr, "Number of points must be between 1 and 1000\n");
        return 1;
    }
    
    printf("\nEvaluating polynomial at %d points from %g to %g:\n", points, start, end);
    double step = (end - start) / (points - 1);
    
    for (int i = 0; i < points; i++) {
        double current_x = start + i * step;
        double value = calculate_polynomial(current_x, coefficients, degree);
        printf("P(%g) = %g\n", current_x, value);
    }
    
    return 0;
}