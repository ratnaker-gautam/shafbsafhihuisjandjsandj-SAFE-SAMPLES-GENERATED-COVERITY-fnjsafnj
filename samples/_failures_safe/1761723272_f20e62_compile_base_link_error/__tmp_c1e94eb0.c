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

int parse_double(const char *str, double *value) {
    char *endptr;
    *value = strtod(str, &endptr);
    if (endptr == str || *endptr != '\0') {
        return 0;
    }
    return 1;
}

int main(void) {
    char input[MAX_INPUT_LEN];
    int degree;
    
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
            fprintf(stderr, "Error reading input\n");
            return 1;
        }
        
        if (!parse_double(input, &coefficients[i])) {
            fprintf(stderr, "Invalid coefficient format\n");
            return 1;
        }
    }
    
    printf("Enter x value to evaluate polynomial: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    double x;
    if (!parse_double(input, &x)) {
        fprintf(stderr, "Invalid x value format\n");
        return 1;
    }
    
    double result = calculate_polynomial(x, coefficients, degree);
    
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
    
    printf("Enter step size for table (0.1-5.0): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    double step;
    if (!parse_double(input, &step)) {
        fprintf(stderr, "Invalid step size format\n");
        return 1;
    }
    
    if (step < 0.1 || step > 5.0) {
        fprintf(stderr, "Step size must be between 0.1 and 5.0\n");
        return 1;
    }
    
    printf("Enter number of table entries (1-20): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    int entries;
    if (sscanf(input, "%d", &entries) != 1) {
        fprintf(stderr, "Invalid entries format\n");
        return 1;
    }
    
    if (entries < 1 || entries > 20) {
        fprintf(stderr, "Entries must be between 1 and 20\n");
        return 1;
    }
    
    printf("\nTable of values:\n");
    printf("x\t\tP(x)\n");
    printf("-------------------\n");
    
    double current_x = x;
    for (int i = 0; i < entries; i++) {
        double table_result = calculate_polynomial(current_x, coefficients, degree);
        printf("%.2f\t\t%.6f\n", current_x, table_result);
        current_x += step;
    }
    
    return 0;
}