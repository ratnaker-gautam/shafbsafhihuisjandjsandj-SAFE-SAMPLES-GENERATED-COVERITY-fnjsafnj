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
    double term = 1.0;
    
    for (int i = 0; i < n; i++) {
        result += coeffs[i] * term;
        term *= x;
    }
    
    return result;
}

int parse_coefficients(char *input, double coeffs[], int max_coeffs) {
    char *token;
    int count = 0;
    
    token = strtok(input, " ");
    while (token != NULL && count < max_coeffs) {
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
    
    return count;
}

int main(void) {
    char input[MAX_INPUT_LEN];
    double coefficients[10];
    double x_values[5];
    int num_coeffs = 0;
    int num_x = 0;
    
    printf("Enter polynomial coefficients (space-separated, max 10): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading coefficients\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "No coefficients provided\n");
        return 1;
    }
    
    char coeff_copy[MAX_INPUT_LEN];
    if (strlen(input) >= sizeof(coeff_copy)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    strcpy(coeff_copy, input);
    
    num_coeffs = parse_coefficients(coeff_copy, coefficients, 10);
    if (num_coeffs <= 0) {
        fprintf(stderr, "Invalid coefficients\n");
        return 1;
    }
    
    printf("Enter x values to evaluate (space-separated, max 5): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading x values\n");
        return 1;
    }
    
    len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "No x values provided\n");
        return 1;
    }
    
    char x_copy[MAX_INPUT_LEN];
    if (strlen(input) >= sizeof(x_copy)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    strcpy(x_copy, input);
    
    char *token = strtok(x_copy, " ");
    while (token != NULL && num_x < 5) {
        char *endptr;
        double x = strtod(token, &endptr);
        
        if (endptr == token || *endptr != '\0') {
            fprintf(stderr, "Invalid x value: %s\n", token);
            return 1;
        }
        
        if (x < -1e100 || x > 1e100) {
            fprintf(stderr, "X value out of range: %s\n", token);
            return 1;
        }
        
        x_values[num_x] = x;
        num_x++;
        token = strtok(NULL, " ");
    }
    
    if (num_x == 0) {
        fprintf(stderr, "No valid x values provided\n");
        return 1;
    }
    
    printf("\nPolynomial evaluation results:\n");
    printf("Coefficients: ");
    for (int i = 0; i < num_coeffs; i++) {
        printf("%.6f ", coefficients[i]);
    }
    printf("\n\n");
    
    for (int i = 0; i < num_x; i++) {
        double result = compute_polynomial(x_values[i], coefficients, num_coeffs);
        if (isnan(result) || isinf(result)) {
            fprintf(stderr, "Numerical error for x = %.6f\n", x_values[i]);
            return 1;
        }
        printf("f(%.6f) = %.6f\n", x_values[i], result);
    }
    
    return 0;
}