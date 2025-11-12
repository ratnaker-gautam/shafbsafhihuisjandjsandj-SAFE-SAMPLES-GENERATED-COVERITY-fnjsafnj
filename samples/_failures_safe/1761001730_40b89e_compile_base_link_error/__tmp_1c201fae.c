//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: numeric_computation
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
    int degree;
    char input[MAX_INPUT_LEN];
    char *endptr;
    
    printf("Enter polynomial coefficients (space-separated, highest degree first): ");
    int coeff_count = read_double_array(coefficients, 10);
    if (coeff_count <= 0) {
        fprintf(stderr, "Error: Invalid coefficients input\n");
        return 1;
    }
    
    degree = coeff_count - 1;
    
    printf("Enter x value to evaluate polynomial: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error: Failed to read x value\n");
        return 1;
    }
    
    x_value = strtod(input, &endptr);
    if (endptr == input || *endptr != '\n') {
        fprintf(stderr, "Error: Invalid x value\n");
        return 1;
    }
    
    printf("Polynomial: ");
    for (int i = degree; i >= 0; i--) {
        if (i == degree) {
            printf("%.2fx^%d", coefficients[i], i);
        } else if (i > 1) {
            printf(" + %.2fx^%d", coefficients[i], i);
        } else if (i == 1) {
            printf(" + %.2fx", coefficients[i]);
        } else {
            printf(" + %.2f", coefficients[i]);
        }
    }
    printf("\n");
    
    double result = calculate_polynomial(x_value, coefficients, degree);
    printf("P(%.2f) = %.6f\n", x_value, result);
    
    printf("Enter x values for table (space-separated): ");
    int table_count = read_double_array(coefficients, 10);
    if (table_count <= 0) {
        fprintf(stderr, "Error: Invalid table values\n");
        return 1;
    }
    
    printf("\nPolynomial Table:\n");
    printf("x\t\tP(x)\n");
    printf("----------------------\n");
    for (int i = 0; i < table_count; i++) {
        double table_result = calculate_polynomial(coefficients[i], coefficients, degree);
        printf("%.2f\t\t%.6f\n", coefficients[i], table_result);
    }
    
    return 0;
}