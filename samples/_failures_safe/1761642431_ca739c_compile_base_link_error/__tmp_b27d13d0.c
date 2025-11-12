//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define MAX_INPUT_LEN 256

double compute_polynomial(double x, double coeffs[], int degree) {
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
    
    token = strtok(input, " ,");
    while (token != NULL && count < max_size) {
        char *endptr;
        double value = strtod(token, &endptr);
        if (endptr == token || *endptr != '\0' && *endptr != '\n' && *endptr != ' ') {
            return -1;
        }
        arr[count] = value;
        count++;
        token = strtok(NULL, " ,");
    }
    
    return count;
}

int main(void) {
    double coefficients[10];
    double x_value;
    char input[MAX_INPUT_LEN];
    int degree;
    
    printf("Enter polynomial coefficients (space/comma separated, max 10): ");
    degree = read_double_array(coefficients, 10) - 1;
    if (degree < 0 || degree > 9) {
        fprintf(stderr, "Error: Invalid coefficients input\n");
        return 1;
    }
    
    printf("Enter x value: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error: Failed to read x value\n");
        return 1;
    }
    
    char *endptr;
    x_value = strtod(input, &endptr);
    if (endptr == input || *endptr != '\0' && *endptr != '\n') {
        fprintf(stderr, "Error: Invalid x value\n");
        return 1;
    }
    
    double result = compute_polynomial(x_value, coefficients, degree);
    
    printf("Polynomial: ");
    for (int i = 0; i <= degree; i++) {
        if (i > 0) printf(" + ");
        printf("%.2f*x^%d", coefficients[i], i);
    }
    printf("\n");
    
    printf("P(%.2f) = %.6f\n", x_value, result);
    
    return 0;
}