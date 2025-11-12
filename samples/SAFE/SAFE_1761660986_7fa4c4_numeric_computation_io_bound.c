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
    double coefficients[10];
    double x_value;
    int num_coeffs;
    char input[MAX_INPUT_LEN];
    char *endptr;
    
    printf("Enter polynomial coefficients (space-separated, max 10): ");
    num_coeffs = read_double_array(coefficients, 10);
    
    if (num_coeffs <= 0) {
        fprintf(stderr, "Error: Invalid coefficients\n");
        return 1;
    }
    
    printf("Enter x value: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error: Failed to read input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = 0;
    x_value = strtod(input, &endptr);
    
    if (endptr == input || *endptr != '\0') {
        fprintf(stderr, "Error: Invalid x value\n");
        return 1;
    }
    
    if (x_value == HUGE_VAL || x_value == -HUGE_VAL) {
        fprintf(stderr, "Error: x value out of range\n");
        return 1;
    }
    
    double result = compute_polynomial(x_value, coefficients, num_coeffs);
    
    if (result == HUGE_VAL || result == -HUGE_VAL) {
        fprintf(stderr, "Error: Result out of range\n");
        return 1;
    }
    
    printf("Polynomial value at x = %.6f: %.6f\n", x_value, result);
    
    return 0;
}