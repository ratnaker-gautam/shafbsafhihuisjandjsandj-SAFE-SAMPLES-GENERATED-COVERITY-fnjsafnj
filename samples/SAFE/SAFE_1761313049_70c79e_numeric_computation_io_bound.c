//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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
    double coefficients[20];
    int degree;
    double x_start, x_end, step;
    char input[MAX_INPUT_LEN];
    
    printf("Enter polynomial degree (0-19): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (sscanf(input, "%d", &degree) != 1) {
        fprintf(stderr, "Invalid degree format\n");
        return 1;
    }
    
    if (degree < 0 || degree > 19) {
        fprintf(stderr, "Degree must be between 0 and 19\n");
        return 1;
    }
    
    printf("Enter %d coefficients (space separated): ", degree + 1);
    int coeff_count = read_double_array(coefficients, degree + 1);
    
    if (coeff_count != degree + 1) {
        fprintf(stderr, "Invalid coefficients\n");
        return 1;
    }
    
    printf("Enter x start value: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (sscanf(input, "%lf", &x_start) != 1) {
        fprintf(stderr, "Invalid start value\n");
        return 1;
    }
    
    printf("Enter x end value: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (sscanf(input, "%lf", &x_end) != 1) {
        fprintf(stderr, "Invalid end value\n");
        return 1;
    }
    
    if (x_end <= x_start) {
        fprintf(stderr, "End value must be greater than start value\n");
        return 1;
    }
    
    printf("Enter step size: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (sscanf(input, "%lf", &step) != 1) {
        fprintf(stderr, "Invalid step size\n");
        return 1;
    }
    
    if (step <= 0.0) {
        fprintf(stderr, "Step size must be positive\n");
        return 1;
    }
    
    printf("\nPolynomial evaluation results:\n");
    printf("x\t\ty\n");
    printf("----------------\n");
    
    double x = x_start;
    int iterations = 0;
    const int max_iterations = 1000;
    
    while (x <= x_end && iterations < max_iterations) {
        double y = compute_polynomial(x, coefficients, degree);
        
        if (!isfinite(y)) {
            fprintf(stderr, "Numerical overflow at x = %f\n", x);
            break;
        }
        
        printf("%.6f\t%.6f\n", x, y);
        
        double next_x = x + step;
        if (next_x <= x) {
            fprintf(stderr, "Step too small, stopping\n");
            break;
        }
        
        x = next_x;
        iterations++;
    }
    
    if (iterations >= max_iterations) {
        fprintf(stderr, "Maximum iterations reached\n");
    }
    
    return 0;
}