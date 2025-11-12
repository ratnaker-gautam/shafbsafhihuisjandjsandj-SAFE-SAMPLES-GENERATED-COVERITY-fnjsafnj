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
    double x_power = 1.0;
    
    for (int i = 0; i < n; i++) {
        result += coeffs[i] * x_power;
        x_power *= x;
    }
    
    return result;
}

int read_coefficients(double coeffs[], int max_coeffs) {
    char input[MAX_INPUT_LEN];
    int count = 0;
    
    printf("Enter polynomial coefficients (space separated, max %d): ", max_coeffs);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    char *token = strtok(input, " \t\n");
    while (token != NULL && count < max_coeffs) {
        char *endptr;
        double value = strtod(token, &endptr);
        
        if (endptr == token || *endptr != '\0') {
            return -1;
        }
        
        if (value < -1e100 || value > 1e100) {
            return -1;
        }
        
        coeffs[count++] = value;
        token = strtok(NULL, " \t\n");
    }
    
    return count;
}

int read_double(const char *prompt, double *value) {
    char input[MAX_INPUT_LEN];
    
    printf("%s", prompt);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 0;
    }
    
    char *endptr;
    *value = strtod(input, &endptr);
    
    if (endptr == input || *endptr != '\n') {
        return 0;
    }
    
    if (*value < -1e100 || *value > 1e100) {
        return 0;
    }
    
    return 1;
}

int main(void) {
    double coefficients[10];
    double x_start, x_end, step;
    int num_coeffs;
    
    printf("Polynomial Function Evaluator\n");
    printf("=============================\n\n");
    
    num_coeffs = read_coefficients(coefficients, 10);
    if (num_coeffs <= 0) {
        fprintf(stderr, "Error: Invalid coefficients input\n");
        return 1;
    }
    
    if (!read_double("Enter start x value: ", &x_start)) {
        fprintf(stderr, "Error: Invalid start value\n");
        return 1;
    }
    
    if (!read_double("Enter end x value: ", &x_end)) {
        fprintf(stderr, "Error: Invalid end value\n");
        return 1;
    }
    
    if (x_end <= x_start) {
        fprintf(stderr, "Error: End value must be greater than start value\n");
        return 1;
    }
    
    if (!read_double("Enter step size: ", &step)) {
        fprintf(stderr, "Error: Invalid step size\n");
        return 1;
    }
    
    if (step <= 0.0 || step > (x_end - x_start)) {
        fprintf(stderr, "Error: Invalid step size\n");
        return 1;
    }
    
    printf("\nEvaluating polynomial: f(x) = ");
    for (int i = 0; i < num_coeffs; i++) {
        if (i > 0) printf(" + ");
        printf("%.2f", coefficients[i]);
        if (i > 0) printf("*x");
        if (i > 1) printf("^%d", i);
    }
    printf("\n\n");
    
    printf("x\t\tf(x)\n");
    printf("----------------------\n");
    
    int steps = 0;
    double x = x_start;
    while (x <= x_end && steps < 1000) {
        double result = compute_polynomial(x, coefficients, num_coeffs);
        
        if (!isfinite(result)) {
            fprintf(stderr, "Error: Numerical overflow at x = %.2f\n", x);
            break;
        }
        
        printf("%.4f\t\t%.6f\n", x, result);
        
        double next_x = x + step;
        if (next_x <= x) {
            break;
        }
        x = next_x;
        steps++;
    }
    
    return 0;
}