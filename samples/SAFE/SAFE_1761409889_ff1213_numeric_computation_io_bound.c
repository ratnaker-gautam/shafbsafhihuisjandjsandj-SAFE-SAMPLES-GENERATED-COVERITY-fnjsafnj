//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100

double compute_polynomial(double x, double coeffs[], int degree) {
    double result = 0.0;
    double power = 1.0;
    
    for (int i = 0; i <= degree; i++) {
        result += coeffs[i] * power;
        power *= x;
    }
    
    return result;
}

int read_coefficients(double coeffs[], int max_degree) {
    char input[MAX_INPUT_LEN + 1];
    char *token;
    int count = 0;
    
    printf("Enter polynomial coefficients (space separated, highest degree first): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    input[strcspn(input, "\n")] = 0;
    
    if (strlen(input) == 0) {
        return -1;
    }
    
    token = strtok(input, " ");
    while (token != NULL && count <= max_degree) {
        char *endptr;
        double value = strtod(token, &endptr);
        
        if (endptr == token || *endptr != '\0') {
            return -1;
        }
        
        if (!isfinite(value)) {
            return -1;
        }
        
        coeffs[count] = value;
        count++;
        token = strtok(NULL, " ");
    }
    
    return count - 1;
}

int main(void) {
    double coefficients[10];
    int degree;
    char input[MAX_INPUT_LEN + 1];
    double x;
    
    printf("Polynomial Evaluator\n");
    printf("===================\n\n");
    
    degree = read_coefficients(coefficients, 9);
    if (degree < 0 || degree > 9) {
        fprintf(stderr, "Error: Invalid coefficients or too many terms (max 10)\n");
        return 1;
    }
    
    printf("Enter x value for evaluation: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error: Failed to read input\n");
        return 1;
    }
    
    char *endptr;
    x = strtod(input, &endptr);
    if (endptr == input || *endptr != '\n' && *endptr != '\0') {
        fprintf(stderr, "Error: Invalid x value\n");
        return 1;
    }
    
    if (!isfinite(x)) {
        fprintf(stderr, "Error: x value is not finite\n");
        return 1;
    }
    
    double result = compute_polynomial(x, coefficients, degree);
    
    if (!isfinite(result)) {
        fprintf(stderr, "Error: Result is not finite\n");
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
    
    printf("\nResult for x = %.2f: %.6f\n", x, result);
    
    return 0;
}