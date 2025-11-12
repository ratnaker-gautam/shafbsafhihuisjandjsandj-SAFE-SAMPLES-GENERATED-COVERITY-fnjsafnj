//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define MAX_INPUT_LEN 256

double compute_polynomial(double x, double coeffs[], int degree) {
    double result = 0.0;
    for (int i = degree; i >= 0; i--) {
        result = result * x + coeffs[i];
    }
    return result;
}

int read_double_array(double arr[], int max_size) {
    char input[MAX_INPUT_LEN];
    char *token;
    int count = 0;
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    token = strtok(input, " \t\n");
    while (token != NULL && count < max_size) {
        char *endptr;
        double value = strtod(token, &endptr);
        if (endptr == token || *endptr != '\0') {
            return -1;
        }
        arr[count++] = value;
    }
    
    return count;
}

int main(void) {
    double coefficients[10];
    double x_value;
    char input[MAX_INPUT_LEN];
    int degree;
    
    printf("Enter polynomial coefficients (space-separated, starting from constant term): ");
    int coeff_count = read_double_array(coefficients, 10);
    if (coeff_count <= 0) {
        fprintf(stderr, "Error: Invalid coefficients input\n");
        return 1;
    }
    
    degree = coeff_count - 1;
    
    printf("Enter x value: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error: Failed to read x value\n");
        return 1;
    }
    
    char *endptr;
    x_value = strtod(input, &endptr);
    if (endptr == input || *endptr != '\n' && *endptr != '\0') {
        fprintf(stderr, "Error: Invalid x value\n");
        return 1;
    }
    
    if (!isfinite(x_value)) {
        fprintf(stderr, "Error: x value is not finite\n");
        return 1;
    }
    
    double result = compute_polynomial(x_value, coefficients, degree);
    
    if (!isfinite(result)) {
        fprintf(stderr, "Error: Computation resulted in non-finite value\n");
        return 1;
    }
    
    printf("Polynomial coefficients: ");
    for (int i = 0; i <= degree; i++) {
        printf("%.6f ", coefficients[i]);
    }
    printf("\n");
    printf("x = %.6f\n", x_value);
    printf("P(x) = %.6f\n", result);
    
    return 0;
}