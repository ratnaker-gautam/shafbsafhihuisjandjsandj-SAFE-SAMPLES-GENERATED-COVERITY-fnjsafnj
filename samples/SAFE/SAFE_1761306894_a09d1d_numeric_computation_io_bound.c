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
        printf("Coefficient %d: ", i);
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            fprintf(stderr, "Error reading coefficient\n");
            return 1;
        }
        
        input[strcspn(input, "\n")] = '\0';
        
        if (!is_valid_double(input, &coefficients[i])) {
            fprintf(stderr, "Invalid coefficient format\n");
            return 1;
        }
    }
    
    printf("Enter x value to evaluate polynomial: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading x value\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    double x;
    if (!is_valid_double(input, &x)) {
        fprintf(stderr, "Invalid x value format\n");
        return 1;
    }
    
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
    
    double result = calculate_polynomial(x, coefficients, degree);
    printf("P(%.2f) = %.6f\n", x, result);
    
    printf("Enter step size for table (positive): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading step size\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    double step;
    if (!is_valid_double(input, &step) || step <= 0) {
        fprintf(stderr, "Invalid step size\n");
        return 1;
    }
    
    printf("Enter number of points (1-20): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading number of points\n");
        return 1;
    }
    
    int num_points;
    if (sscanf(input, "%d", &num_points) != 1) {
        fprintf(stderr, "Invalid number of points\n");
        return 1;
    }
    
    if (num_points < 1 || num_points > 20) {
        fprintf(stderr, "Number of points must be between 1 and 20\n");
        return 1;
    }
    
    printf("\nTable of values:\n");
    printf("x\t\tP(x)\n");
    printf("-------------------\n");
    
    double current_x = x;
    for (int i = 0; i < num_points; i++) {
        double value = calculate_polynomial(current_x, coefficients, degree);
        printf("%.4f\t%.6f\n", current_x, value);
        current_x += step;
    }
    
    return 0;
}