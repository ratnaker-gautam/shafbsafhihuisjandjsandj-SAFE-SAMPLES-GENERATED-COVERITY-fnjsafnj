//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define MAX_INPUT_LEN 256

double compute_polynomial(double x, double coeffs[], int degree) {
    double result = 0.0;
    for (int i = 0; i <= degree; i++) {
        result += coeffs[i] * pow(x, i);
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

int main() {
    char input[MAX_INPUT_LEN];
    double coefficients[10];
    double x_value;
    int degree;
    
    printf("Polynomial Evaluator\n");
    printf("Enter polynomial coefficients (space/comma separated, highest degree first): ");
    
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
    
    degree = parse_coefficients(input, coefficients, 9);
    if (degree < 0) {
        fprintf(stderr, "Invalid coefficient format\n");
        return 1;
    }
    
    printf("Enter x value to evaluate: ");
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
    
    double result = compute_polynomial(x_value, coefficients, degree);
    
    if (!isfinite(result)) {
        fprintf(stderr, "Result is not finite\n");
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
    
    printf("P(%.2f) = %.6f\n", x_value, result);
    
    return 0;
}