//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_LEN 256

double compute_polynomial(double x, double coeffs[], int degree) {
    double result = 0.0;
    double term = 1.0;
    
    for (int i = 0; i <= degree; i++) {
        result += coeffs[i] * term;
        term *= x;
    }
    
    return result;
}

int parse_coefficients(char *input, double coeffs[], int max_degree) {
    char *token;
    int count = 0;
    
    token = strtok(input, " ");
    while (token != NULL && count <= max_degree) {
        char *endptr;
        double value = strtod(token, &endptr);
        
        if (endptr == token || *endptr != '\0') {
            return -1;
        }
        
        if (value < -1e100 || value > 1e100) {
            return -1;
        }
        
        coeffs[count] = value;
        count++;
        token = strtok(NULL, " ");
    }
    
    return count - 1;
}

int main() {
    char input[MAX_INPUT_LEN];
    double coefficients[10];
    double x_values[5];
    int degree;
    
    printf("Enter polynomial coefficients (space-separated, max 10): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    degree = parse_coefficients(input, coefficients, 9);
    if (degree < 0 || degree > 9) {
        fprintf(stderr, "Invalid coefficients\n");
        return 1;
    }
    
    printf("Enter 5 x values to evaluate (space-separated): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    char *token = strtok(input, " ");
    int x_count = 0;
    
    while (token != NULL && x_count < 5) {
        char *endptr;
        double x = strtod(token, &endptr);
        
        if (endptr == token || *endptr != '\0') {
            fprintf(stderr, "Invalid x value\n");
            return 1;
        }
        
        if (x < -1e100 || x > 1e100) {
            fprintf(stderr, "X value out of range\n");
            return 1;
        }
        
        x_values[x_count] = x;
        x_count++;
        token = strtok(NULL, " ");
    }
    
    if (x_count != 5) {
        fprintf(stderr, "Need exactly 5 x values\n");
        return 1;
    }
    
    printf("\nPolynomial evaluation results:\n");
    printf("Degree: %d\n", degree);
    printf("Coefficients: ");
    for (int i = 0; i <= degree; i++) {
        printf("%.3f ", coefficients[i]);
    }
    printf("\n\n");
    
    for (int i = 0; i < 5; i++) {
        double result = compute_polynomial(x_values[i], coefficients, degree);
        if (isnan(result) || isinf(result)) {
            printf("x = %.3f: Computation error\n", x_values[i]);
        } else {
            printf("x = %.3f: f(x) = %.6f\n", x_values[i], result);
        }
    }
    
    return 0;
}