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
    double coeffs[20];
    double x;
    int num_coeffs;
    
    printf("Polynomial Evaluator\n");
    printf("Enter polynomial coefficients (space-separated, highest degree first): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    char coeff_input[MAX_INPUT_LEN];
    strncpy(coeff_input, input, sizeof(coeff_input) - 1);
    coeff_input[sizeof(coeff_input) - 1] = '\0';
    
    num_coeffs = parse_coefficients(coeff_input, coeffs, 20);
    if (num_coeffs <= 0) {
        fprintf(stderr, "Invalid coefficients\n");
        return 1;
    }
    
    printf("Enter x value: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *endptr;
    x = strtod(input, &endptr);
    if (endptr == input || *endptr != '\n' && *endptr != '\0') {
        fprintf(stderr, "Invalid x value\n");
        return 1;
    }
    
    if (x < -1e100 || x > 1e100) {
        fprintf(stderr, "X value out of range\n");
        return 1;
    }
    
    double result = compute_polynomial(x, coeffs, num_coeffs);
    
    if (isnan(result) || isinf(result)) {
        fprintf(stderr, "Result is not finite\n");
        return 1;
    }
    
    printf("Polynomial: ");
    for (int i = 0; i < num_coeffs; i++) {
        if (i > 0) printf(" + ");
        printf("%.2f*x^%d", coeffs[i], num_coeffs - i - 1);
    }
    printf("\n");
    
    printf("P(%.2f) = %.6f\n", x, result);
    
    return 0;
}