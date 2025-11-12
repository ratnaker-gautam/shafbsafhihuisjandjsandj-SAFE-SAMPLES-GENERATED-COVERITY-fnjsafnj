//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_LEN 256

double compute_polynomial(double x, double coeffs[], int n) {
    double result = 0.0;
    double x_power = 1.0;
    
    for (int i = 0; i < n; i++) {
        result += coeffs[i] * x_power;
        x_power *= x;
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
        fprintf(stderr, "Error: Invalid coefficients input\n");
        return 1;
    }
    
    printf("Enter x value: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error: Failed to read x value\n");
        return 1;
    }
    
    x_value = strtod(input, &endptr);
    if (endptr == input || *endptr != '\n' && *endptr != '\0') {
        fprintf(stderr, "Error: Invalid x value\n");
        return 1;
    }
    
    double result = compute_polynomial(x_value, coefficients, num_coeffs);
    
    printf("Polynomial coefficients: ");
    for (int i = 0; i < num_coeffs; i++) {
        printf("%.6f ", coefficients[i]);
    }
    printf("\n");
    
    printf("P(%.6f) = %.6f\n", x_value, result);
    
    return 0;
}