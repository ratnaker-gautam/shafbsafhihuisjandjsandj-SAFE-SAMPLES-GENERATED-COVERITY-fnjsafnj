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

int read_double_array(double arr[], int max_size) {
    char input[MAX_INPUT_LEN];
    char *token;
    int count = 0;
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    input[strcspn(input, "\n")] = 0;
    
    token = strtok(input, " ");
    while (token != NULL && count < max_size) {
        char *endptr;
        double value = strtod(token, &endptr);
        
        if (endptr == token || *endptr != '\0') {
            return -1;
        }
        
        if (value == HUGE_VAL || value == -HUGE_VAL) {
            return -1;
        }
        
        arr[count] = value;
        count++;
        token = strtok(NULL, " ");
    }
    
    return count;
}

int main(void) {
    double coefficients[20];
    double x_value;
    int degree;
    char input[MAX_INPUT_LEN];
    char *endptr;
    
    printf("Enter polynomial coefficients (space-separated, highest degree first): ");
    int coeff_count = read_double_array(coefficients, 20);
    
    if (coeff_count <= 0 || coeff_count > 20) {
        fprintf(stderr, "Error: Invalid coefficients input\n");
        return 1;
    }
    
    degree = coeff_count - 1;
    
    printf("Enter x value to evaluate polynomial: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error: Failed to read x value\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = 0;
    x_value = strtod(input, &endptr);
    
    if (endptr == input || *endptr != '\0') {
        fprintf(stderr, "Error: Invalid x value\n");
        return 1;
    }
    
    if (x_value == HUGE_VAL || x_value == -HUGE_VAL) {
        fprintf(stderr, "Error: X value out of range\n");
        return 1;
    }
    
    double result = compute_polynomial(x_value, coefficients, degree);
    
    if (result == HUGE_VAL || result == -HUGE_VAL) {
        fprintf(stderr, "Error: Result overflow\n");
        return 1;
    }
    
    printf("Polynomial degree: %d\n", degree);
    printf("P(%.6f) = %.6f\n", x_value, result);
    
    return 0;
}