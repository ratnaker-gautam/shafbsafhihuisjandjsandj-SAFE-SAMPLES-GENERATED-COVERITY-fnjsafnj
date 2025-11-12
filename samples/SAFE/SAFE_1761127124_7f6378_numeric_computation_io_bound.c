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
    double x_value;
    int degree;
    
    printf("Polynomial Calculator\n");
    printf("Enter polynomial coefficients (space-separated, highest degree first): ");
    
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
    
    if (x_value < -1e100 || x_value > 1e100) {
        fprintf(stderr, "X value out of range\n");
        return 1;
    }
    
    double result = calculate_polynomial(x_value, coefficients, degree);
    
    if (isnan(result) || isinf(result)) {
        fprintf(stderr, "Result is not finite\n");
        return 1;
    }
    
    printf("P(%.6f) = %.6f\n", x_value, result);
    
    printf("Evaluating polynomial at 10 points in range [-2, 2]:\n");
    for (int i = 0; i < 10; i++) {
        double x = -2.0 + i * 0.4;
        double y = calculate_polynomial(x, coefficients, degree);
        
        if (!isnan(y) && !isinf(y)) {
            printf("P(%.2f) = %.6f\n", x, y);
        } else {
            printf("P(%.2f) = undefined\n", x);
        }
    }
    
    return 0;
}