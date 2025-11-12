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
    for (int i = 0; i < n; i++) {
        result += coeffs[i] * pow(x, i);
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
        double val = strtod(token, &endptr);
        if (endptr == token || *endptr != '\0') {
            return -1;
        }
        arr[count++] = val;
        token = strtok(NULL, " \t\n");
    }
    
    return count;
}

int main(void) {
    double coefficients[10];
    double x;
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
        fprintf(stderr, "Error: Failed to read x value\n");
        return 1;
    }
    
    x = strtod(input, &endptr);
    if (endptr == input || *endptr != '\n' && *endptr != '\0') {
        fprintf(stderr, "Error: Invalid x value\n");
        return 1;
    }
    
    if (isnan(x) || isinf(x)) {
        fprintf(stderr, "Error: x value is not finite\n");
        return 1;
    }
    
    double result = compute_polynomial(x, coefficients, num_coeffs);
    if (isnan(result) || isinf(result)) {
        fprintf(stderr, "Error: Computation resulted in invalid value\n");
        return 1;
    }
    
    printf("P(%.6f) = %.6f\n", x, result);
    
    return 0;
}