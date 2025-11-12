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
    
    token = strtok(input, " ,");
    while (token != NULL && count <= max_degree) {
        char *endptr;
        double value = strtod(token, &endptr);
        
        if (endptr == token || *endptr != '\0') {
            return -1;
        }
        
        coeffs[count] = value;
        count++;
        token = strtok(NULL, " ,");
    }
    
    return count - 1;
}

int main(void) {
    char input[MAX_INPUT_LEN];
    double coefficients[10];
    double x_values[5];
    int degree;
    
    printf("Polynomial Evaluator\n");
    printf("Enter polynomial coefficients (space/comma separated, max 10): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    degree = parse_coefficients(input, coefficients, 9);
    if (degree < 0 || degree > 9) {
        fprintf(stderr, "Invalid coefficients\n");
        return 1;
    }
    
    printf("Polynomial degree: %d\n", degree);
    printf("Enter 5 x-values to evaluate (space separated): ");
    
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
            fprintf(stderr, "Invalid x-value\n");
            return 1;
        }
        
        if (!isfinite(x)) {
            fprintf(stderr, "Invalid x-value\n");
            return 1;
        }
        
        x_values[x_count] = x;
        x_count++;
        token = strtok(NULL, " ");
    }
    
    if (x_count == 0) {
        fprintf(stderr, "No valid x-values provided\n");
        return 1;
    }
    
    printf("\nResults:\n");
    printf("x\t\tf(x)\n");
    printf("--\t\t----\n");
    
    for (int i = 0; i < x_count; i++) {
        double result = compute_polynomial(x_values[i], coefficients, degree);
        if (!isfinite(result)) {
            printf("%.6f\tOverflow/NaN\n", x_values[i]);
        } else {
            printf("%.6f\t%.6f\n", x_values[i], result);
        }
    }
    
    return 0;
}