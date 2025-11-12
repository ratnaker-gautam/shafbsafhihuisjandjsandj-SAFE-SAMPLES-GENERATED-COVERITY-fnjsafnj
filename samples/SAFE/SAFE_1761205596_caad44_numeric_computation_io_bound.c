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
    
    input[strcspn(input, "\n")] = 0;
    
    token = strtok(input, " ");
    while (token != NULL && count <= max_degree) {
        char *endptr;
        double value = strtod(token, &endptr);
        
        if (endptr == token || *endptr != '\0') {
            printf("Invalid coefficient: %s\n", token);
            return -1;
        }
        
        if (!isfinite(value)) {
            printf("Coefficient out of range: %s\n", token);
            return -1;
        }
        
        coeffs[count] = value;
        count++;
        token = strtok(NULL, " ");
    }
    
    return count - 1;
}

int main(void) {
    double coefficients[20];
    int degree;
    char input[MAX_INPUT_LEN];
    double x;
    
    degree = read_coefficients(coefficients, 19);
    if (degree < 0) {
        fprintf(stderr, "Error reading coefficients\n");
        return 1;
    }
    
    printf("Polynomial degree: %d\n", degree);
    printf("Enter x value for evaluation: ");
    
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
    
    if (!isfinite(x)) {
        fprintf(stderr, "x value out of range\n");
        return 1;
    }
    
    double result = compute_polynomial(x, coefficients, degree);
    
    if (!isfinite(result)) {
        fprintf(stderr, "Result is not finite\n");
        return 1;
    }
    
    printf("P(%.6f) = %.6f\n", x, result);
    
    printf("Enter step size for table generation: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    double step;
    endptr = NULL;
    step = strtod(input, &endptr);
    if (endptr == input || *endptr != '\n' && *endptr != '\0') {
        fprintf(stderr, "Invalid step size\n");
        return 1;
    }
    
    if (step <= 0.0 || !isfinite(step)) {
        fprintf(stderr, "Step size must be positive and finite\n");
        return 1;
    }
    
    printf("Enter number of table entries: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    long num_entries;
    endptr = NULL;
    num_entries = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\n' && *endptr != '\0' || num_entries <= 0 || num_entries > 1000) {
        fprintf(stderr, "Invalid number of entries (1-1000)\n");
        return 1;
    }
    
    printf("\nPolynomial evaluation table:\n");
    printf("x\t\tP(x)\n");
    printf("-------------------\n");
    
    double current_x = x;
    for (long i = 0; i < num_entries; i++) {
        double value = compute_polynomial(current_x, coefficients, degree);
        if (isfinite(value)) {
            printf("%.6f\t%.6f\n", current_x, value);
        } else {
            printf("%.6f\tundefined\n", current_x);
        }
        current_x += step;
    }
    
    return 0;
}