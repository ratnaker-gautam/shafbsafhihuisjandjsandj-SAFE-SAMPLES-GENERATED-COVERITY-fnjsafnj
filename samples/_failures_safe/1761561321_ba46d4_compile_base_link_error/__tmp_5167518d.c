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
    
    double result = compute_polynomial(x, coefficients, degree);
    
    printf("Polynomial: ");
    for (int i = 0; i <= degree; i++) {
        if (i == 0) {
            printf("%.2f", coefficients[i]);
        } else if (i == 1) {
            printf(" + %.2fx", coefficients[i]);
        } else {
            printf(" + %.2fx^%d", coefficients[i], i);
        }
    }
    printf("\n");
    
    printf("P(%.2f) = %.6f\n", x, result);
    
    printf("Enter start, end, and step for range evaluation:\n");
    double start, end, step;
    
    printf("Start: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading start value\n");
        return 1;
    }
    if (sscanf(input, "%lf", &start) != 1) {
        fprintf(stderr, "Invalid start value\n");
        return 1;
    }
    
    printf("End: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading end value\n");
        return 1;
    }
    if (sscanf(input, "%lf", &end) != 1) {
        fprintf(stderr, "Invalid end value\n");
        return 1;
    }
    
    printf("Step: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading step value\n");
        return 1;
    }
    if (sscanf(input, "%lf", &step) != 1) {
        fprintf(stderr, "Invalid step value\n");
        return 1;
    }
    
    if (step <= 0.0 || start > end) {
        fprintf(stderr, "Invalid range parameters\n");
        return 1;
    }
    
    printf("\nRange evaluation:\n");
    printf("x\t\tP(x)\n");
    printf("-----------------------\n");
    
    int points = 0;
    for (double current = start; current <= end && points < 1000; current += step) {
        double value = compute_polynomial(current, coefficients, degree);
        printf("%.3f\t\t%.6f\n", current, value);
        points++;
    }
    
    return 0;
}