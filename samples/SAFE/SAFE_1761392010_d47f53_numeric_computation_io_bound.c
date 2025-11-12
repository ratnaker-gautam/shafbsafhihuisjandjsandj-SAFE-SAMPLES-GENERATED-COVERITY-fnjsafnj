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
        
        if (value < -1e100 || value > 1e100) {
            return -1;
        }
        
        arr[count] = value;
        count++;
        token = strtok(NULL, " ");
    }
    
    return count;
}

int main(void) {
    double coefficients[10];
    double x;
    int degree;
    char input[MAX_INPUT_LEN];
    char *endptr;
    
    printf("Enter polynomial coefficients (space-separated, max 10): ");
    int coeff_count = read_double_array(coefficients, 10);
    if (coeff_count <= 0) {
        fprintf(stderr, "Error: Invalid coefficients\n");
        return 1;
    }
    
    degree = coeff_count - 1;
    
    printf("Enter x value: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error: Failed to read input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = 0;
    x = strtod(input, &endptr);
    if (endptr == input || *endptr != '\0') {
        fprintf(stderr, "Error: Invalid x value\n");
        return 1;
    }
    
    if (x < -1e100 || x > 1e100) {
        fprintf(stderr, "Error: x value out of range\n");
        return 1;
    }
    
    double result = compute_polynomial(x, coefficients, degree);
    
    if (isnan(result) || isinf(result)) {
        fprintf(stderr, "Error: Computation resulted in invalid value\n");
        return 1;
    }
    
    printf("Polynomial: ");
    for (int i = 0; i <= degree; i++) {
        if (i == 0) {
            printf("%.2f", coefficients[i]);
        } else if (i == 1) {
            printf(" + %.2fx", coefficients[i]);
        } else {
            printf(" + %.2fx^%d", coefficients[i], i);
        }
    }
    printf("\n");
    
    printf("P(%.2f) = %.6f\n", x, result);
    
    return 0;
}