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

int main() {
    double coefficients[10];
    double x_value;
    char input[MAX_INPUT_LEN];
    int degree;
    
    printf("Enter polynomial coefficients (space/comma separated, max 10): ");
    int coeff_count = read_double_array(coefficients, 10);
    if (coeff_count <= 0) {
        fprintf(stderr, "Error: Invalid coefficients\n");
        return 1;
    }
    
    degree = coeff_count - 1;
    
    printf("Enter x value: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    char *endptr;
    x_value = strtod(input, &endptr);
    if (endptr == input || *endptr != '\0' && *endptr != '\n') {
        fprintf(stderr, "Error: Invalid x value\n");
        return 1;
    }
    
    printf("Polynomial: ");
    for (int i = degree; i >= 0; i--) {
        if (i == degree) {
            printf("%.2f", coefficients[i]);
        } else if (coefficients[i] > 0) {
            printf(" + %.2f", coefficients[i]);
        } else if (coefficients[i] < 0) {
            printf(" - %.2f", -coefficients[i]);
        }
        
        if (i > 1) {
            printf("x^%d", i);
        } else if (i == 1) {
            printf("x");
        }
    }
    printf("\n");
    
    double result = compute_polynomial(x_value, coefficients, degree);
    printf("P(%.2f) = %.6f\n", x_value, result);
    
    return 0;
}