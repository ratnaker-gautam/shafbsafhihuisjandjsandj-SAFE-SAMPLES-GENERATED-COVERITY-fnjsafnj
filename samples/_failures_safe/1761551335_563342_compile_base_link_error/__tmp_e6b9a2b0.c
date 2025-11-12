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
    for (int i = 0; i <= degree; i++) {
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
    
    token = strtok(input, " ");
    while (token != NULL && count < max_size) {
        char *endptr;
        double value = strtod(token, &endptr);
        if (endptr == token || *endptr != '\0' && *endptr != '\n') {
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
    int degree;
    double x_value;
    char input[MAX_INPUT_LEN];
    char *endptr;
    
    printf("Enter polynomial degree (0-9): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    degree = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\0' && *endptr != '\n') {
        fprintf(stderr, "Invalid degree format\n");
        return 1;
    }
    if (degree < 0 || degree > 9) {
        fprintf(stderr, "Degree must be between 0 and 9\n");
        return 1;
    }
    
    printf("Enter %d coefficients (space separated): ", degree + 1);
    int coeff_count = read_double_array(coefficients, degree + 1);
    if (coeff_count != degree + 1) {
        fprintf(stderr, "Invalid coefficients format\n");
        return 1;
    }
    
    printf("Enter x value: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    x_value = strtod(input, &endptr);
    if (endptr == input || *endptr != '\0' && *endptr != '\n') {
        fprintf(stderr, "Invalid x value format\n");
        return 1;
    }
    
    double result = calculate_polynomial(x_value, coefficients, degree);
    printf("P(%.2f) = %.6f\n", x_value, result);
    
    return 0;
}