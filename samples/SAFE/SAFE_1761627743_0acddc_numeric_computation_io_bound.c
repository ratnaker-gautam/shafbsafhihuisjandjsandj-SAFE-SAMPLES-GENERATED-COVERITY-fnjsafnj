//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define MAX_INPUT_LEN 256

double compute_polynomial(double x, double coeffs[], int degree) {
    double result = 0.0;
    for (int i = degree; i >= 0; i--) {
        result = result * x + coeffs[i];
    }
    return result;
}

int read_coefficients(double coeffs[], int max_degree) {
    char input[MAX_INPUT_LEN];
    int degree = -1;
    
    while (degree < 0 || degree > max_degree) {
        printf("Enter polynomial degree (0-%d): ", max_degree);
        if (fgets(input, sizeof(input), stdin) == NULL) {
            return -1;
        }
        
        char *endptr;
        long temp = strtol(input, &endptr, 10);
        if (endptr == input || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (temp < 0 || temp > max_degree) {
            printf("Degree must be between 0 and %d.\n", max_degree);
            continue;
        }
        
        degree = (int)temp;
    }
    
    printf("Enter %d coefficients (from constant term to highest degree):\n", degree + 1);
    for (int i = 0; i <= degree; i++) {
        int valid = 0;
        while (!valid) {
            printf("Coefficient for x^%d: ", i);
            if (fgets(input, sizeof(input), stdin) == NULL) {
                return -1;
            }
            
            char *endptr;
            coeffs[i] = strtod(input, &endptr);
            if (endptr == input || (*endptr != '\n' && *endptr != '\0')) {
                printf("Invalid input. Please enter a number.\n");
                continue;
            }
            valid = 1;
        }
    }
    
    return degree;
}

int main(void) {
    const int max_degree = 10;
    double coefficients[max_degree + 1];
    char input[MAX_INPUT_LEN];
    
    printf("Polynomial Evaluator\n");
    printf("===================\n\n");
    
    int degree = read_coefficients(coefficients, max_degree);
    if (degree < 0) {
        fprintf(stderr, "Error reading coefficients.\n");
        return 1;
    }
    
    printf("\nPolynomial: ");
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
    printf("\n\n");
    
    while (1) {
        printf("Enter x value to evaluate (or 'q' to quit): ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        if (input[0] == 'q' && (input[1] == '\n' || input[1] == '\0')) {
            break;
        }
        
        char *endptr;
        double x = strtod(input, &endptr);
        if (endptr == input || (*endptr != '\n' && *endptr != '\0')) {
            printf("Invalid input. Please enter a number or 'q' to quit.\n");
            continue;
        }
        
        double result = compute_polynomial(x, coefficients, degree);
        printf("P(%.2f) = %.6f\n\n", x, result);
    }
    
    printf("Goodbye!\n");
    return 0;
}