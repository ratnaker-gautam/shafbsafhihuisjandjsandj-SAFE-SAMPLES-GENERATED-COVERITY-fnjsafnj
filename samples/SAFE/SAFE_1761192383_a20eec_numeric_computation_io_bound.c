//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define MAX_INPUT_LEN 256

double compute_polynomial(double x, double coeffs[], int degree) {
    double result = 0.0;
    double power = 1.0;
    
    for (int i = 0; i <= degree; i++) {
        result += coeffs[i] * power;
        power *= x;
    }
    
    return result;
}

int read_coefficients(double coeffs[], int max_degree) {
    char input[MAX_INPUT_LEN];
    char *token;
    int count = 0;
    
    printf("Enter polynomial coefficients (space separated, highest degree first): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    token = strtok(input, " \t\n");
    while (token != NULL && count <= max_degree) {
        char *endptr;
        double value = strtod(token, &endptr);
        
        if (endptr == token || *endptr != '\0') {
            printf("Invalid coefficient: %s\n", token);
            return -1;
        }
        
        if (!isfinite(value)) {
            printf("Coefficient is not finite: %s\n", token);
            return -1;
        }
        
        coeffs[count] = value;
        count++;
        token = strtok(NULL, " \t\n");
    }
    
    return count - 1;
}

int main(void) {
    double coefficients[10];
    int degree;
    char input[MAX_INPUT_LEN];
    double x;
    
    degree = read_coefficients(coefficients, 9);
    if (degree < 0) {
        fprintf(stderr, "Error reading coefficients\n");
        return 1;
    }
    
    printf("Polynomial degree: %d\n", degree);
    printf("Enter x value to evaluate polynomial: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading x value\n");
        return 1;
    }
    
    char *endptr;
    x = strtod(input, &endptr);
    if (endptr == input || *endptr != '\n') {
        fprintf(stderr, "Invalid x value\n");
        return 1;
    }
    
    if (!isfinite(x)) {
        fprintf(stderr, "x value is not finite\n");
        return 1;
    }
    
    double result = compute_polynomial(x, coefficients, degree);
    
    if (!isfinite(result)) {
        fprintf(stderr, "Result is not finite\n");
        return 1;
    }
    
    printf("P(%.6f) = %.6f\n", x, result);
    
    printf("Enter x values to evaluate (type 'quit' to exit):\n");
    
    while (1) {
        printf("x = ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        if (strncmp(input, "quit", 4) == 0) {
            break;
        }
        
        x = strtod(input, &endptr);
        if (endptr == input || (*endptr != '\n' && *endptr != '\0')) {
            printf("Invalid input\n");
            continue;
        }
        
        if (!isfinite(x)) {
            printf("x value is not finite\n");
            continue;
        }
        
        result = compute_polynomial(x, coefficients, degree);
        
        if (!isfinite(result)) {
            printf("Result is not finite\n");
            continue;
        }
        
        printf("P(%.6f) = %.6f\n", x, result);
    }
    
    return 0;
}