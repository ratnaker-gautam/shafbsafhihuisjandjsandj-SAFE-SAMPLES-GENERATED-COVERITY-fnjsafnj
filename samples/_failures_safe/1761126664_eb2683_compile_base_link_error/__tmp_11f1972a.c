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
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    char *token = strtok(input, " ");
    int count = 0;
    
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
        token = strtok(NULL, " ");
    }
    
    return count;
}

int main(void) {
    double coefficients[10];
    int degree;
    double x_value;
    
    printf("Enter polynomial degree (0-9): ");
    if (scanf("%d", &degree) != 1) {
        fprintf(stderr, "Invalid degree input\n");
        return 1;
    }
    
    if (degree < 0 || degree > 9) {
        fprintf(stderr, "Degree must be between 0 and 9\n");
        return 1;
    }
    
    while (getchar() != '\n');
    
    printf("Enter %d coefficients (space separated): ", degree + 1);
    int coeff_count = read_double_array(coefficients, degree + 1);
    if (coeff_count != degree + 1) {
        fprintf(stderr, "Invalid coefficients input\n");
        return 1;
    }
    
    printf("Enter x value: ");
    if (scanf("%lf", &x_value) != 1) {
        fprintf(stderr, "Invalid x value\n");
        return 1;
    }
    
    if (x_value == HUGE_VAL || x_value == -HUGE_VAL) {
        fprintf(stderr, "X value out of range\n");
        return 1;
    }
    
    double result = calculate_polynomial(x_value, coefficients, degree);
    
    if (result == HUGE_VAL || result == -HUGE_VAL) {
        fprintf(stderr, "Result out of range\n");
        return 1;
    }
    
    printf("P(%.2f) = %.6f\n", x_value, result);
    
    printf("Evaluating polynomial at 10 points from -2 to 2:\n");
    for (int i = 0; i < 10; i++) {
        double x = -2.0 + i * 0.4;
        double y = calculate_polynomial(x, coefficients, degree);
        if (y != HUGE_VAL && y != -HUGE_VAL) {
            printf("P(%.2f) = %.6f\n", x, y);
        }
    }
    
    return 0;
}