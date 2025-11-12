//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_LEN 256

double compute_polynomial(double x, double coeffs[], int n) {
    double result = 0.0;
    for (int i = 0; i < n; i++) {
        result += coeffs[i] * pow(x, n - 1 - i);
    }
    return result;
}

int read_coefficients(double coeffs[], int max_degree) {
    char input[MAX_INPUT_LEN];
    printf("Enter polynomial coefficients (space separated, highest degree first): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    char *token = strtok(input, " \t\n");
    int count = 0;
    
    while (token != NULL && count <= max_degree) {
        char *endptr;
        double value = strtod(token, &endptr);
        
        if (endptr == token || *endptr != '\0') {
            return -1;
        }
        
        coeffs[count] = value;
        count++;
        token = strtok(NULL, " \t\n");
    }
    
    return count;
}

int main(void) {
    const int max_degree = 10;
    double coefficients[max_degree + 1];
    char input[MAX_INPUT_LEN];
    
    printf("Polynomial Evaluator\n");
    printf("===================\n\n");
    
    int degree = read_coefficients(coefficients, max_degree);
    if (degree <= 0) {
        fprintf(stderr, "Error: Invalid coefficients input\n");
        return 1;
    }
    
    printf("Enter x value for evaluation: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error: Failed to read input\n");
        return 1;
    }
    
    char *endptr;
    double x = strtod(input, &endptr);
    if (endptr == input || *endptr != '\n' && *endptr != '\0') {
        fprintf(stderr, "Error: Invalid x value\n");
        return 1;
    }
    
    printf("Polynomial: ");
    for (int i = 0; i < degree; i++) {
        if (i > 0) {
            printf(" + ");
        }
        printf("%.2fx^%d", coefficients[i], degree - 1 - i);
    }
    printf("\n");
    
    double result = compute_polynomial(x, coefficients, degree);
    
    printf("P(%.2f) = %.6f\n", x, result);
    
    printf("\nEnter range for evaluation (start end step): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error: Failed to read input\n");
        return 1;
    }
    
    double start, end, step;
    int parsed = sscanf(input, "%lf %lf %lf", &start, &end, &step);
    if (parsed != 3 || step <= 0.0 || start > end) {
        fprintf(stderr, "Error: Invalid range specification\n");
        return 1;
    }
    
    printf("\nRange evaluation from %.2f to %.2f with step %.2f:\n", start, end, step);
    printf("x\t\tP(x)\n");
    printf("-----------------------\n");
    
    double current = start;
    int evaluations = 0;
    const int max_evaluations = 1000;
    
    while (current <= end && evaluations < max_evaluations) {
        double value = compute_polynomial(current, coefficients, degree);
        printf("%.2f\t\t%.6f\n", current, value);
        current += step;
        evaluations++;
    }
    
    if (evaluations >= max_evaluations) {
        fprintf(stderr, "Warning: Maximum evaluations reached\n");
    }
    
    return 0;
}