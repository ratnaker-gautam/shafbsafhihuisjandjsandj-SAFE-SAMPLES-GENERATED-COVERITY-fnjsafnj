//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define MAX_INPUT_LEN 256

double compute_polynomial(double x, double coeffs[], int degree) {
    double result = 0.0;
    double x_power = 1.0;
    
    for (int i = 0; i <= degree; i++) {
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
    
    token = strtok(input, " ,");
    while (token != NULL && count < max_size) {
        char *endptr;
        double value = strtod(token, &endptr);
        
        if (endptr == token || *endptr != '\0' && *endptr != '\n' && *endptr != ' ') {
            return -1;
        }
        
        if (value == HUGE_VAL || value == -HUGE_VAL) {
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
    int degree;
    char input[MAX_INPUT_LEN];
    char *endptr;
    
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
    
    x_value = strtod(input, &endptr);
    if (endptr == input || *endptr != '\0' && *endptr != '\n') {
        fprintf(stderr, "Error: Invalid x value\n");
        return 1;
    }
    
    if (x_value == HUGE_VAL || x_value == -HUGE_VAL) {
        fprintf(stderr, "Error: X value out of range\n");
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
    
    double result = compute_polynomial(x_value, coefficients, degree);
    
    if (result == HUGE_VAL || result == -HUGE_VAL) {
        fprintf(stderr, "Error: Result overflow\n");
        return 1;
    }
    
    printf("P(%.2f) = %.6f\n", x_value, result);
    
    return 0;
}