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
    
    printf("Enter x value to evaluate: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    double x;
    if (sscanf(input, "%lf", &x) != 1) {
        fprintf(stderr, "Invalid x value format\n");
        return 1;
    }
    
    printf("Polynomial: ");
    for (int i = degree; i >= 0; i--) {
        if (i == degree) {
            printf("%.2fx^%d", coefficients[i], i);
        } else if (i > 1) {
            printf(" + %.2fx^%d", coefficients[i], i);
        } else if (i == 1) {
            printf(" + %.2fx", coefficients[i]);
        } else {
            printf(" + %.2f", coefficients[i]);
        }
    }
    printf("\n");
    
    double result = calculate_polynomial(x, coefficients, degree);
    printf("Result at x = %.2f: %.6f\n", x, result);
    
    printf("Enter start, end, and step for range evaluation: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    double start, end, step;
    if (sscanf(input, "%lf %lf %lf", &start, &end, &step) != 3) {
        fprintf(stderr, "Invalid range format\n");
        return 1;
    }
    
    if (step <= 0 || start > end) {
        fprintf(stderr, "Invalid range parameters\n");
        return 1;
    }
    
    printf("Range evaluation from %.2f to %.2f with step %.2f:\n", start, end, step);
    printf("x\t\tf(x)\n");
    printf("-------------------\n");
    
    double current = start;
    while (current <= end) {
        double value = calculate_polynomial(current, coefficients, degree);
        printf("%.2f\t\t%.6f\n", current, value);
        current += step;
    }
    
    return 0;
}