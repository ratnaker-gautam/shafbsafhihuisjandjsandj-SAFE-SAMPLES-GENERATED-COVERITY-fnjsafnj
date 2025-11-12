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
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    char *token = strtok(input, " \t\n");
    int count = 0;
    
    while (token != NULL && count < max_size) {
        char *endptr;
        double value = strtod(token, &endptr);
        if (endptr == token || *endptr != '\0') {
            return -1;
        }
        arr[count] = value;
        count++;
        token = strtok(NULL, " \t\n");
    }
    
    return count;
}

int main(void) {
    double coefficients[10];
    int degree;
    double x_value;
    
    printf("Enter polynomial coefficients (space-separated, highest degree first): ");
    int num_coeffs = read_double_array(coefficients, 10);
    if (num_coeffs <= 0) {
        fprintf(stderr, "Error: Invalid coefficients input\n");
        return 1;
    }
    
    degree = num_coeffs - 1;
    
    printf("Enter x value for evaluation: ");
    char x_input[MAX_INPUT_LEN];
    if (fgets(x_input, sizeof(x_input), stdin) == NULL) {
        fprintf(stderr, "Error: Invalid x value input\n");
        return 1;
    }
    
    char *endptr;
    x_value = strtod(x_input, &endptr);
    if (endptr == x_input || *endptr != '\n' && *endptr != '\0') {
        fprintf(stderr, "Error: Invalid x value format\n");
        return 1;
    }
    
    printf("Polynomial: ");
    for (int i = degree; i >= 0; i--) {
        if (i == degree) {
            printf("%.2f", coefficients[degree - i]);
        } else if (i > 1) {
            printf(" + %.2fx^%d", coefficients[degree - i], i);
        } else if (i == 1) {
            printf(" + %.2fx", coefficients[degree - i]);
        } else {
            printf(" + %.2f", coefficients[degree - i]);
        }
    }
    printf("\n");
    
    double result = compute_polynomial(x_value, coefficients, degree);
    printf("P(%.2f) = %.6f\n", x_value, result);
    
    printf("Evaluating polynomial at 10 points from %.2f to %.2f:\n", x_value - 2.0, x_value + 2.0);
    for (int i = 0; i < 10; i++) {
        double eval_x = x_value - 2.0 + (4.0 * i) / 9.0;
        double eval_result = compute_polynomial(eval_x, coefficients, degree);
        printf("P(%.2f) = %.6f\n", eval_x, eval_result);
    }
    
    return 0;
}