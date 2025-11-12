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
    double x_power = 1.0;
    
    for (int i = 0; i < n; i++) {
        result += coeffs[i] * x_power;
        x_power *= x;
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
        
        coeffs[count] = value;
        count++;
        token = strtok(NULL, " ");
    }
    
    return count;
}

int main() {
    char input[MAX_INPUT_LEN];
    double coefficients[10];
    double x_value;
    int num_coeffs;
    
    printf("Polynomial Evaluator\n");
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
        fprintf(stderr, "Invalid coefficients format\n");
        return 1;
    }
    
    printf("Enter x value: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *endptr;
    x_value = strtod(input, &endptr);
    if (endptr == input || *endptr != '\n' && *endptr != '\0') {
        fprintf(stderr, "Invalid x value\n");
        return 1;
    }
    
    if (!isfinite(x_value)) {
        fprintf(stderr, "x value is not finite\n");
        return 1;
    }
    
    double result = compute_polynomial(x_value, coefficients, num_coeffs);
    
    if (!isfinite(result)) {
        fprintf(stderr, "Result is not finite\n");
        return 1;
    }
    
    printf("Polynomial: ");
    for (int i = 0; i < num_coeffs; i++) {
        if (i > 0) printf(" + ");
        printf("%.2f*x^%d", coefficients[i], i);
    }
    printf("\n");
    
    printf("Result for x = %.2f: %.6f\n", x_value, result);
    
    return 0;
}