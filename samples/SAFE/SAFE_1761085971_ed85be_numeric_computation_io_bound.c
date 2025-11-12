//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: numeric_computation
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
    
    input[strcspn(input, "\n")] = '\0';
    
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
    int degree;
    double x_value;
    char input[MAX_INPUT_LEN];
    char *endptr;
    
    printf("Enter polynomial degree (0-9): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    degree = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\0' || degree < 0 || degree > 9) {
        fprintf(stderr, "Invalid degree\n");
        return 1;
    }
    
    printf("Enter %d coefficients separated by spaces: ", degree + 1);
    int coeff_count = read_double_array(coefficients, degree + 1);
    if (coeff_count != degree + 1) {
        fprintf(stderr, "Invalid coefficients\n");
        return 1;
    }
    
    printf("Enter x value: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    x_value = strtod(input, &endptr);
    if (endptr == input || *endptr != '\0') {
        fprintf(stderr, "Invalid x value\n");
        return 1;
    }
    
    if (x_value == HUGE_VAL || x_value == -HUGE_VAL) {
        fprintf(stderr, "X value out of range\n");
        return 1;
    }
    
    double result = compute_polynomial(x_value, coefficients, degree);
    
    printf("Polynomial: ");
    for (int i = 0; i <= degree; i++) {
        if (i == 0) {
            printf("%.2f", coefficients[i]);
        } else {
            printf(" + %.2f*x^%d", coefficients[i], i);
        }
    }
    printf("\n");
    
    printf("P(%.2f) = %.6f\n", x_value, result);
    
    return 0;
}